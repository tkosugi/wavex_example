CFLAG_OPTIM := -O3

NODE_NAME := $(shell uname -n)
ifeq (${NODE_NAME}, sekirei1)
	CC_SERIAL := g++
	CC_MPI := g++
	CFLAG_OMP := -fopenmp
	CFLAG_OTHER := -std=c++1z
	LIB_OTHER_COMMON := -lmkl_gf_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lgfortran
	LIB_OTHER_MPI := -lmpi_mt -lmpigf
else
	CC_SERIAL := gcc-7.1
	CC_MPI := mpicxx
	CFLAG_OMP := -fopenmp
	CFLAG_OTHER := -std=c++1z
	LIB_OTHER_COMMON := -lm -lstdc++ -llapack -lblas -lgfortran
	LIB_OTHER_MPI := -lmpi_mpifh
endif

DIR_WAVEX := $(HOME)/ccsd
INC_WAVEX := $(DIR_WAVEX)/include
LIB_WAVEX_SERIAL := $(DIR_WAVEX)/lib/wavex_serial.a 
LIB_WAVEX_MPI := $(DIR_WAVEX)/lib/wavex_mpi.a 
LIB_WAVEX_MPI_OMP := $(DIR_WAVEX)/lib/wavex_mpi_omp.a 

####################################################################################

CC_OPTION := $(CFLAG_OPTIM) $(CFLAG_OTHER)

LIB_COMMON := $(LIB_OTHER_COMMON)
LIB_SERIAL := $(LIB_COMMON) $(LIB_WAVEX_SERIAL)
LIB_MPI := $(LIB_COMMON) $(LIB_OTHER_MPI) $(LIB_WAVEX_MPI)
LIB_MPI_OMP := $(LIB_COMMON) $(LIB_OTHER_MPI) $(LIB_WAVEX_MPI_OMP) 

TARGET_NAME := run_wavex

DIR_BIN := bin
DIR_SRC := src
DIR_INC := $(DIR_SRC)/include $(INC_WAVEX) $(INC_OTHER)
DIR_OBJ := obj
DIR_OBJ_SERIAL := $(DIR_OBJ)/serial
DIR_OBJ_MPI := $(DIR_OBJ)/mpi
DIR_OBJ_MPI_OMP := $(DIR_OBJ)/mpi_omp

SRC_CPP := $(shell find $(DIR_SRC) -name "*.cpp")

OBJ_SERIAL := $(SRC_CPP:$(DIR_SRC)/%.cpp=$(DIR_OBJ_SERIAL)/%.o) 
OBJ_MPI := $(SRC_CPP:$(DIR_SRC)/%.cpp=$(DIR_OBJ_MPI)/%.o)  
OBJ_MPI_OMP := $(SRC_CPP:$(DIR_SRC)/%.cpp=$(DIR_OBJ_MPI_OMP)/%.o)

DIR_TO_BE_MADE := $(DIR_BIN) $(sort $(dir $(OBJ_SERIAL) $(OBJ_MPI) $(OBJ_MPI_OMP)))

TARGET_SERIAL := $(DIR_BIN)/$(TARGET_NAME)_serial.exe
TARGET_MPI := $(DIR_BIN)/$(TARGET_NAME)_mpi.exe
TARGET_MPI_OMP := $(DIR_BIN)/$(TARGET_NAME)_mpi_omp.exe

OPTION_INC := $(DIR_INC:%=-I%)
OPTION_DEP := -MMD -MF
MPI_FLAG := MPI_USED

.PHONY: all
all: \
	show_msg \
	$(DIR_TO_BE_MADE) \
	$(TARGET_SERIAL) \
	$(TARGET_MPI) \
	$(TARGET_MPI_OMP) \
	TAGS

.PHONY: show_msg
show_msg:
	$(info node name: $(NODE_NAME))
	$(info compiler for serial: $(CC_SERIAL))
	$(info compiler for MPI: $(CC_MPI))
	$(info )

$(DIR_TO_BE_MADE):
	mkdir -p $@

.PHONY: TAGS
TAGS: $(SRC_CPP)
	etags $^ -o $(DIR_SRC)/TAGS

$(TARGET_SERIAL): $(OBJ_SERIAL)
	@echo "linking for serial..."
	$(CC_SERIAL) -o $@ $^ $(CC_OPTION) \
	$(LIB_SERIAL)

$(TARGET_MPI): $(OBJ_MPI)
	@echo "linking for MPI..."
	$(CC_MPI) -o $@ $^ $(CC_OPTION) \
	$(LIB_MPI)

$(TARGET_MPI_OMP): $(OBJ_MPI_OMP)
	@echo "linking for MPI+OpenMP..."
	$(CC_MPI) -o $@ $^ $(CC_OPTION) $(CFLAG_OMP) \
	$(LIB_MPI_OMP)

$(DIR_OBJ_SERIAL)/%.o: $(DIR_SRC)/%.cpp
	@[ -d $(DIR_OBJ_SERIAL) ]
	$(CC_SERIAL) -o $@ -c $< \
	$(CC_OPTION) $(OPTION_DEP)$(@:%.o=%.d) \
	$(OPTION_INC) \

$(DIR_OBJ_MPI)/%.o: $(DIR_SRC)/%.cpp
	@[ -d $(DIR_OBJ_MPI) ]
	$(CC_MPI) -o $@ -c $< \
	$(CC_OPTION) $(OPTION_DEP)$(@:%.o=%.d) \
	$(OPTION_INC) \
	-D$(MPI_FLAG)

$(DIR_OBJ_MPI_OMP)/%.o: $(DIR_SRC)/%.cpp
	@[ -d $(DIR_OBJ_MPI_OMP) ]
	$(CC_MPI) -o $@ -c $< \
	$(CC_OPTION) $(CFLAG_OMP) $(OPTION_DEP)$(@:%.o=%.d) \
	$(OPTION_INC) \
	-D$(MPI_FLAG)

.PHONY: clean
clean:
	$(RM) -rf \
	$(DIR_OBJ) \
	$(DIR_BIN)

-include $(OBJ_SERIAL:.o=.d)
-include $(OBJ_MPI:.o=.d)
-include $(OBJ_MPI_OMP:.o=.d)
