# Copyright (c) 1993-1995 Argonaut Technologies Limited. All rights reserved.
#
# $Id: softizf.mak 1.2 1997/02/16 20:15:53 jon Exp $
# $Locker: $
#
# Makefile for autoloaded primitives (Indexed Z)
#
.IMPORT: BR_SOURCE_DIR BR_MAKEFILE BR_TARGET_DIR BR_MAKE_DIR


TARGET_TYPE*:=DRIVER

LIB_INC:=drv

.IF $(TARGET_TYPE) == LIB

BLD_BASE:=lizf
LIB_PART:=lizf

.ELIF $(TARGET_TYPE) == DRIVER

BLD_BASE:=sizf
TARGET=softizf$(LIB_SUFFIX_T).dll
EXPORTS:=softizf.exf
EXPORT_TYPE:=1
CFLAGS+=
LDLIBS+=\
	$(DDI_LIB_DIR)/fwimg$(LIB_SUFFIX_C)$(LIB_EXT)\
	$(LIB_DIR)/brmt$(LIB_TYPE)$(LIB_EXT)\

.ENDIF

BLD_DIR=$(BLD_BASE)$(LIB_TYPE)

CFLAGS+=-DPARTS=PART_8Z_FOG -DBASED_$(BR_BASE_TYPE)=1
ASFLAGS+=-DPARTS=PART_8Z_FOG -DBASED_$(BR_BASE_TYPE)=1

## Base library object files
##
OBJS_C=\
	$(BLD_DIR)/work$(OBJ_EXT)\
	$(BLD_DIR)/frcp$(OBJ_EXT)\
	$(BLD_DIR)/rcp$(OBJ_EXT)\
	$(BLD_DIR)/dither$(OBJ_EXT)\
\
	$(BLD_DIR)/perspz$(OBJ_EXT)\

OBJS_ASM=\
	$(BLD_DIR)/safediv$(OBJ_EXT)\
	$(BLD_DIR)/sar16$(OBJ_EXT)\
	$(BLD_DIR)/magicsym$(OBJ_EXT)\
\
	$(BLD_DIR)/ti8_pizp$(OBJ_EXT)\
\
        $(BLD_DIR)/zb8$(OBJ_EXT)\
        $(BLD_DIR)/zb8p2lit$(OBJ_EXT)\
        $(BLD_DIR)/zb8p2fla$(OBJ_EXT)\
        $(BLD_DIR)/zb8p2unl$(OBJ_EXT)\
        $(BLD_DIR)/zb8awtm$(OBJ_EXT)\
        $(BLD_DIR)/decal$(OBJ_EXT)\
        $(BLD_DIR)/fpsetup$(OBJ_EXT)\
        $(BLD_DIR)/fti8_piz$(OBJ_EXT)\
\
        $(BLD_DIR)/pfpsetup$(OBJ_EXT)\
        $(BLD_DIR)/fti8pizp$(OBJ_EXT)\

.INCLUDE: $(BR_MAKE_DIR)/$(BR_MAKEFILE)

