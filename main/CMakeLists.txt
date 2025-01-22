idf_component_register(SRCS "main.c" 
                            "peripherals/buzz.c" 
                            "peripherals/rgb.c" 
                            "sensors/lightSensor.c" 
                            "peripherals/button.c" 
                            "screen/animations.c" 
                            "screen/animation_play.c" 
                            "screen/display_text.c"
                    INCLUDE_DIRS "." "./peripherals" "./sensors" "./screen")
