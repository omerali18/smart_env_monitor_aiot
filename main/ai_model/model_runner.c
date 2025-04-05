#include "model_runner.h"
#include "model_data.h"

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "esp_log.h"

#define TAG "TFLITE"

// Memory arena for the interpreter (adjust if needed)
constexpr int kTensorArenaSize = 2 * 1024;
static uint8_t tensor_arena[kTensorArenaSize];

// Globals
static tflite::MicroInterpreter* interpreter = nullptr;
static TfLiteTensor* input = nullptr;
static TfLiteTensor* output = nullptr;

void ai_model_init() {
    static tflite::MicroErrorReporter micro_error_reporter;
    const tflite::Model* model = tflite::GetModel(model_data);
    
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        ESP_LOGE(TAG, "Model schema mismatch!");
        return;
    }

    static tflite::AllOpsResolver resolver;
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, &micro_error_reporter);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        ESP_LOGE(TAG, "Tensor allocation failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    ESP_LOGI(TAG, "TinyML model initialized");
}

bool run_inference(float temperature, float humidity) {
    if (!input || input->dims->size != 2 || input->dims->data[1] != 2) {
        ESP_LOGE(TAG, "Model input tensor not set up correctly.");
        return false;
    }

    input->data.f[0] = temperature;
    input->data.f[1] = humidity;

    if (interpreter->Invoke() != kTfLiteOk) {
        ESP_LOGE(TAG, "Failed to run inference");
        return false;
    }

    float prediction = output->data.f[0];  // Assume output is 0-1 float
    ESP_LOGI(TAG, "AI Prediction: %.3f", prediction);

    return (prediction > 0.5f);  // Binary anomaly: >0.5 means anomaly
}
