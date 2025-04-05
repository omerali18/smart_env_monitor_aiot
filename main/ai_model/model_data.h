#ifndef MODEL_DATA_H_
#define MODEL_DATA_H_

// Dummy TFLite model as byte array (replace later with real model)
const unsigned char model_data[] = {
  0x20, 0x00, 0x00, 0x00, // Just a placeholder
  0x54, 0x46, 0x4C, 0x33,
  // Normally you'd paste 100s of bytes here
};

const int model_data_len = sizeof(model_data);

#endif  // MODEL_DATA_H_
