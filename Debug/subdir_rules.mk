################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/localhome/calib3/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/bin/cl430" -vmsp --use_hw_mpy=16 --include_path="/localhome/calib3/ti/ccs901/ccs/ccs_base/msp430/include" --include_path="/usr/include/" --include_path="/localhome/calib3/workspace_v9/frmware_v5_22_1" --include_path="/localhome/calib3/ti/ccs901/ccs/tools/compiler/ti-cgt-msp430_18.12.1.LTS/include" --advice:power=all --define=__MSP430F169__ -g --c11 --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


