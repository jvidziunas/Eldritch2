/*==================================================================*\
  WrenVm.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

#define WREN_NAN_TAGGING 1
#define WREN_OPT_RANDOM 0
#define WREN_OPT_META 0

// HACK: Symbol conflicts with Windows headers. Works because we're compiling all the Wren code in this TU.
#define TokenType WrenTokenType

ET_PUSH_COMPILER_WARNING_STATE()
/*	(4100) lots of unreferenced parameters.
 *	(4127) MSVC doesn't like some of the compile-time constant comparison Wren does.
 *	(4702) Wren includes a few default switch cases, etc. that are documented as being unreachable.
 *	(4200) Wren makes fairly extensive use of flexible/zero-size arrays.
 *	(4709) Wren bytecode interpreter uses the comma operator in instruction macros to combine multiple operations into one line.
 *	(4996) Wren uses POSIX CRT functions instead of Microsoft's Secure CRT for cross-platform compatibility.
 *	(4244) Signed/unsigned type mismatches/conversions.
 *	(6340) Signed/unsigned char conversion in printf.
 *	(6011) Static analyzer freakout re: null pointers in WrenVM struct.
 *	(4701, 4703)  MSVC doesn't pick up on certain Wren functions initializing 'out' parameters through pointers. */
	ET_SET_MSVC_WARNING_STATE(disable : 4100 4127 4702 4200 4709 4996 4244 6340 6011 4701 4703)
#	include <wren/src/vm/wren_primitive.c>
#	include <wren/src/vm/wren_compiler.c>
#	include <wren/src/vm/wren_debug.c>
#	include <wren/src/vm/wren_utils.c>
#	include <wren/src/vm/wren_value.c>
#	include <wren/src/vm/wren_core.c>
#	include <wren/src/vm/wren_vm.c>
	ET_POP_COMPILER_WARNING_STATE()