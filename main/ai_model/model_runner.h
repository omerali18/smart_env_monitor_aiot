#ifndef MODEL_RUNNER_H_
#define MODEL_RUNNER_H_

#ifdef __cplusplus
extern "C" {
#endif

// Initialize AI model
void ai_model_init();

// Run inference: returns true if anomaly detected
bool run_inference(float temperature, float humidity);

#ifdef __cplusplus
}
#endif

#endif // MODEL_RUNNER_H_
