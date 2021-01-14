################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
drivers/eMPL/%.obj: ../drivers/eMPL/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"E:/ccs/Ti/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="E:/ccs/Ti/ccs/ccs_base/arm/include" --include_path="E:/ccs/workplace7/mpu6050/drivers/eMPL" --include_path="E:/ccs/workplace7/mpu6050/drivers/include" --include_path="E:/ccs/workplace7/mpu6050/msp432Peripherals/ESP" --include_path="E:/ccs/workplace7/mpu6050/drivers" --include_path="E:/ccs/MPU6050_msp432-master/msp432Peripherals/mpuUart" --include_path="E:/ccs/workplace7/mpu6050/eMPL-hal" --include_path="E:/ccs/MPU6050_msp432-master/mllite" --include_path="E:/ccs/workplace7/mpu6050/mpl" --include_path="E:/ccs/workplace7/mpu6050/msp432Peripherals" --include_path="E:/ccs/workplace7/mpu6050/delay" --include_path="E:/ccs/workplace7/mpu6050/driverlib" --include_path="E:/ccs/Ti/ccs/ccs_base/arm/include/CMSIS" --include_path="E:/ccs/workplace7/mpu6050" --include_path="E:/ccs/Ti/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=MPU6050 --define=MOTION_DRIVER_TARGET_MSP432 --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="drivers/eMPL/$(basename $(<F)).d_raw" --obj_directory="drivers/eMPL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


