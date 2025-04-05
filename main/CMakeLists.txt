idf_component_register(SRCS 
    "app_main.c"
    "app_wifi.c"
    "oled_display.c"
    "sensor_task.c"
    "gpio_control.c"
    "tinyml_ai.c"
    "ota_update.c"
    "ai_model/model_runner.c"
    INCLUDE_DIRS "." "ai_model"
    PRIV_REQUIRES tflite)
