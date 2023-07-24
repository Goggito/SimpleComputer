CC = gcc
CFLAGS = -Wall -MMD -I.
RM = rm -rf *.o *.d *.a *.txt 

LIB_DIR = lib
SRC_DIR = src
OBJ_DIR = obj
TRANS_DIR = translator

MAIN = main
SUB = interface.c cell.c accum.c big.c usersignal.c timer.c key.c ALU.c CU.c


SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TEST1 = test_lab1
TEST2 = test_lab2
TEST3 = test_lab3
TEST4 = test_lab4

LIB_mSC = mySimpleComputer
LIB_mT = myTerm
LIB_mBC = myBigChars
LIB_mRk = myReadkey

all: $(MAIN) sat sbt 

#MAIN
$(MAIN): $(OBJS) lib$(LIB_mSC).a lib$(LIB_mT).a lib$(LIB_mBC).a lib$(LIB_mRk).a
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) -L. -I. -l$(LIB_mSC) -l$(LIB_mT) -l$(LIB_mBC) -l$(LIB_mRk) -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# LIB
#mySimpleComputer
lib$(LIB_mSC).a: $(LIB_DIR)/$(LIB_mSC)/$(LIB_mSC).o
	ar cr lib$(LIB_mSC).a $(LIB_DIR)/$(LIB_mSC)/$(LIB_mSC).o 

$(LIB_DIR)/$(LIB_mSC).o: $(LIB_DIR)/$(LIB_mSC)/$(LIB_mSC).c 
	$(CC) $(CFLAGS) -c $(LIB_DIR)/$(LIB_mSC)/$(LIB_mSC).c 

#myTerm
lib$(LIB_mT).a: $(LIB_DIR)/$(LIB_mT)/$(LIB_mT).o
	ar cr lib$(LIB_mT).a $(LIB_DIR)/$(LIB_mT)/$(LIB_mT).o 

$(LIB_DIR)/$(LIB_mT).o: $(LIB_DIR)/$(LIB_mT)/$(LIB_mT).c 
	$(CC) $(CFLAGS) -c $(LIB_DIR)/$(LIB_mT)/$(LIB_mT).c 

#myBigChars
lib$(LIB_mBC).a: $(LIB_DIR)/$(LIB_mBC)/$(LIB_mBC).o 
	ar cr lib$(LIB_mBC).a $(LIB_DIR)/$(LIB_mBC)/$(LIB_mBC).o 

$(LIB_DIR)/$(LIB_mBC).o: $(LIB_DIR)/$(LIB_mBC)/$(LIB_mBC).c lib$(LIB_mT).a
	$(CC) $(CFLAGS) -c $(LIB_DIR)/$(LIB_mBC)/$(LIB_mBC).c -lm -L -l$(LIB_mT)

#myTerm
lib$(LIB_mRk).a: $(LIB_DIR)/$(LIB_mRk)/$(LIB_mRk).o
	ar cr lib$(LIB_mRk).a $(LIB_DIR)/$(LIB_mRk)/$(LIB_mRk).o 

$(LIB_DIR)/$(LIB_mRk).o: $(LIB_DIR)/$(LIB_mRk)/$(LIB_mRk).c 
	$(CC) $(CFLAGS) -c $(LIB_DIR)/$(LIB_mRk)/$(LIB_mRk).c 

# TRANSLATOR
sat: $(OBJ_DIR)/sat.o  
	$(CC) -o $@ $< 
 
sbt: $(OBJ_DIR)/sbt.o $(TRANS_DIR)/rpnTranslator.h 
	$(CC) -o $@ $< 

$(OBJ_DIR)/sat.o: $(TRANS_DIR)/sat.c
	$(CC) $(CFLAGS) -c $< -o $@
 
$(OBJ_DIR)/sbt.o: $(TRANS_DIR)/sbt.c $(TRANS_DIR)/rpnTranslator.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/rpnTranslator.o: $(TRANS_DIR)/rpnTranslator.c $(TRANS_DIR)/rpnTranslator.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) obj/sat.o obj/sbt.o
	$(RM) $(MAIN)
	find $(LIB_DIR)/$(LIB_mSC) -name '*.o' -exec $(RM) '{}' \;
	find $(LIB_DIR)/$(LIB_mT) -name '*.o' -exec $(RM) '{}' \;
	find $(LIB_DIR)/$(LIB_mBC) -name '*.o' -exec $(RM) '{}' \;
	find $(LIB_DIR)/$(LIB_mRk) -name '*.o' -exec $(RM) '{}' \;
