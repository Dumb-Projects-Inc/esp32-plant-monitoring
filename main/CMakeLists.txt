idf_component_register(SRCS "main.c" 
                            "peripherals/buzz.c" 
                            "peripherals/rgb.c" 
                            "sensors/lightSensor.c" 
                            "peripherals/button.c" 
                            "screen/animation_play.c" 
                            "screen/screen_manager.c"
                            "screen/bitmap.c"
                    INCLUDE_DIRS "." "./peripherals" "./sensors" "./screen")
