################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/eMPL/%.obj: ../drivers/eMPL/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/ccs/ccs_base/arm/include" --include_path="D:/workplace msp430/output/mpu6050/drivers/eMPL" --include_path="D:/workplace msp430/output/mpu6050/drivers/include" --include_path="D:/workplace msp430/output/mpu6050/msp432Peripherals/ESP" --include_path="D:/workplace msp430/output/mpu6050/drivers" --include_path="D:/workplace msp430/output/mpu6050/eMPL-hal" --include_path="D:/workplace msp430/output/mpu6050/mpl" --include_path="D:/workplace msp430/output/mpu6050/msp432Peripherals" --include_path="D:/workplace msp430/output/mpu6050/delay" --include_path="D:/workplace msp430/output/mpu6050/driverlib" --include_path="D:/ccs/ccs_base/arm/include/CMSIS" --include_path="D:/workplace msp430/output/mpu6050" --include_path="D:/ccs/tools/compiler/ti-cgt-arm_20.2.1.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=MPU6050 --define=MOTION_DRIVER_TARGET_MSP432 --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="drivers/eMPL/$(basename $(<F)).d_raw" --obj_directory="drivers/eMPL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


