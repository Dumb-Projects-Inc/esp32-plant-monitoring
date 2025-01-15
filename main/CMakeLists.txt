<<<<<<< HEAD
idf_component_register(SRCS "main.c" "peripherals/buzz.c" "peripherals/rgb.c" "sensors/lightSensor.c" "peripherals/button.c"
                    INCLUDE_DIRS "." "./peripherals" "./sensors"
                    REQUIRES am2320 stemma_soil )
=======
idf_component_register(SRCS "main.c" 
                            "peripherals/buzz.c" 
                            "peripherals/rgb.c" 
                            "sensors/lightSensor.c" 
                            "peripherals/button.c" 
                            "screen/animations.c" 
                            "screen/leaf_animation.c" 
                            "screen/settings_animation.c"
                            "layouts/humidity_screen.c"
                            "screen/display_text.c"
                    INCLUDE_DIRS "." "./peripherals" "./sensors" "./layouts" "./screen")
>>>>>>> 749b086 (screen, layot and display animation and text functions and stuff)
