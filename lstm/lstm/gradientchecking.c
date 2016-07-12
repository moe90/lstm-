FLOAT calculateLoss(graph_t *graph, output_t *output, int outputLayerSize, int T, int j) {
  FLOAT loss; 
  
  loss = output->lossFunction(output->target[T][j], graph->layers[graph->numLayers-1].y[T][j], outputLayerSize);

  return loss;
}

FLOAT checkWeights(graph_t *graph, FLOAT *w, int m, int n, int T, int j, FLOAT *ref, int isOutputLayer) {
  int a, b, outputLayerSize = graph->layers[graph->numLayers-1].layerSize;
  FLOAT Temp, difference, maxDifference, loss1, loss2;
  output_t *output;  
  
  outputLayerSize = graph->layers[graph->numLayers-1].layerSize;
  maxDifference = 0;
  loss1 = 0;
  loss2 = 0;
  output = graph->layers[graph->numLayers-1].outputLayer;
  
  for(a = 0; a < m; a++) {
    for(b = 0; b < n; b++) { 
      Temp = w[a*n+b];     
      
      //loss1
      w[a*n+b] = Temp + EPSILON;
      if(isOutputLayer == true) {
        forwardPassOutputLayer(graph, T, j); 
      } else {        
        forwardPass(graph, graph->currentSequence[j], j); 
      }        
      loss1 = calculateLoss(graph, output, outputLayerSize, T, j);
      
      //loss2
      w[a*n+b] = Temp - EPSILON;
      if(isOutputLayer == true) {
        forwardPassOutputLayer(graph, T, j); 
      } else {
        forwardPass(graph, graph->currentSequence[j], j);
      }
      loss2 = calculateLoss(graph, output, outputLayerSize, T, j);
 
      w[a*n+b] = Temp;
      difference = ((loss1-loss2)/(2*EPSILON)-ref[a*n+b]);
      difference = difference < 0 ? -difference : difference; 
      if(difference > maxDifference) {
        maxDifference = difference;
      }
    }
  }
  
  forwardPass(graph, graph->currentSequence[j], j);
  printf("%d %d %e\n", T, isOutputLayer, maxDifference);
  return maxDifference;
}

FLOAT checkGradientLstmLayer(graph_t *graph, layer_t *layer, int T, int j) {
  int i, layerSize, inputSize, cur = layer->cur;
  FLOAT difference[12], maxDifference;
  lstm_t *lstm = layer->lstmLayer;
  
  layerSize = layer->layerSize;
  inputSize = layer->inputSize;  
  maxDifference = 0;
  
  for(i = 0; i < 12; i++) {
    difference[i] = 0;
  }  
  
  i = 0; 

  difference[i++] = checkWeights(graph, lstm->Wz, layerSize, inputSize, T, j, lstm->dWz[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->Wi, layerSize, inputSize, T, j, lstm->dWi[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->Wf, layerSize, inputSize, T, j, lstm->dWf[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->Wo, layerSize, inputSize, T, j, lstm->dWo[cur][j], false);  
  if(T != 0) {
    difference[i++] = checkWeights(graph, lstm->Rz, layerSize, layerSize, T, j, lstm->dRz[cur][j], false);
    difference[i++] = checkWeights(graph, lstm->Ri, layerSize, layerSize, T, j, lstm->dRi[cur][j], false);
    difference[i++] = checkWeights(graph, lstm->Rf, layerSize, layerSize, T, j, lstm->dRf[cur][j], false);
    difference[i++] = checkWeights(graph, lstm->Ro, layerSize, layerSize, T, j, lstm->dRo[cur][j], false); 
  }  
  difference[i++] = checkWeights(graph, lstm->bz, layerSize, 1, T, j, lstm->dbz[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->bi, layerSize, 1, T, j, lstm->dbi[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->bf, layerSize, 1, T, j, lstm->dbf[cur][j], false);
  difference[i++] = checkWeights(graph, lstm->bo, layerSize, 1, T, j, lstm->dbo[cur][j], false);
        
  for(i = 0; i < 12; i++) {
    maxDifference = difference[i] > maxDifference ? difference[i] : maxDifference;    
  } 
  return maxDifference;
}

FLOAT checkGradientOutputLayer(graph_t *graph, layer_t *layer, int T, int j) {
  int i, layerSize, inputSize, cur;
  FLOAT difference[3], maxDifference;
  output_t *output = layer->outputLayer;
  
  cur = layer->cur;
  layerSize = layer->layerSize;
  inputSize = layer->inputSize;
  maxDifference = 0;

  for(i = 0; i < 3; i++) {
    difference[i] = 0;
  }  
  
  i = 0;
  
  difference[i++] = checkWeights(graph, output->W,      layerSize, inputSize, T, j, output->dW[cur][T][j], true);
  difference[i++] = checkWeights(graph, output->b,      layerSize, 1,         T, j, output->db[cur][T][j], true);
  difference[i++] = checkWeights(graph, layer->x[T][j], inputSize, 1,         T, j, layer->dx[T][j],  true);
  
  for(i = 0; i < 3; i++) {
    maxDifference = difference[i] > maxDifference ? difference[i] : maxDifference;    
  } 
  return maxDifference;
}
 
FLOAT checkGradientLayer(graph_t *graph, layer_t *layer, int T, int j) {
  FLOAT difference = 0;
  
  switch(layer->type) {
    case INPUT_LAYER:
      break;
    case LSTM_LAYER:
      difference = checkGradientLstmLayer(graph, layer, T, j);
      printf("%d lstm dif = %e\n", T, difference);
      break;
    case OUTPUT_LAYER:
      difference =  checkGradientOutputLayer(graph, layer, T, j);
      printf("%d out dif = %e\n", T, difference);
      break;
    default:
      printError("invalid layer type in gradient check", stderr, true);      
  } 
  return difference;  
}

FLOAT checkGradient(graph_t *graph, int j) {
  int i, T = graph->currentSequence[j]->length-1, numLayers = graph->numLayers;
  FLOAT difference, maxDifference = 0;

  if(graph->outputPer == TIMESTEP) {
    printError("gradient checking implemented for SEQUENCE mode", stderr, false);
  }
  
  for(i = 0; i < numLayers; i++) {
    printf("layer %d\n", i);
    difference = checkGradientLayer(graph, &(graph->layers[i]), T, j);
    maxDifference = difference > maxDifference ? difference : maxDifference;
  }
  
  return maxDifference;
}