################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
software/watchdog/%.obj: ../software/watchdog/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="D:/ccs/ccs_base/msp430/include" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/hardware/ADS112C04" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/hardware/hardware_ic" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/MSP430F5xx_6xx" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/software/mathclub" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/hardware/exti_interrupt" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/hardware/oled" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final/software/watchdog" --include_path="D:/workplace msp430/output/Digital multimeter_5529/MSP430F5xx_6xx" --include_path="D:/workplace msp430/output/5529/Digital multimeter_5529final" --include_path="D:/ccs/tools/compiler/ti-cgt-msp430_20.2.1.LTS/include" --advice:power=all --define=__MSP430F5529__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="software/watchdog/$(basename $(<F)).d_raw" --obj_directory="software/watchdog" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


