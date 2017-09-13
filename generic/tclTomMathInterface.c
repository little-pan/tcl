/*
 *----------------------------------------------------------------------
 *
 * tclTomMathInterface.c --
 *
 *	This file contains procedures that are used as a 'glue' layer between
 *	Tcl and libtommath.
 *
 * Copyright (c) 2005 by Kevin B. Kenny.  All rights reserved.
 *
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

#include "tclInt.h"
#include "tommath.h"

MODULE_SCOPE const TclTomMathStubs tclTomMathStubs;

/*
 *----------------------------------------------------------------------
 *
 * TclTommath_Init --
 *
 *	Initializes the TclTomMath 'package', which exists as a
 *	placeholder so that the package data can be used to hold
 *	a stub table pointer.
 *
 * Results:
 *	Returns a standard Tcl result.
 *
 * Side effects:
 *	Installs the stub table for tommath.
 *
 *----------------------------------------------------------------------
 */

int
TclTommath_Init(
    Tcl_Interp *interp)		/* Tcl interpreter */
{
    /* TIP #268: Full patchlevel instead of just major.minor */

    if (Tcl_PkgProvideEx(interp, "tcl::tommath", TCL_PATCH_LEVEL,
	    &tclTomMathStubs) != TCL_OK) {
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclBN_epoch --
 *
 *	Return the epoch number of the TclTomMath stubs table
 *
 * Results:
 *	Returns an arbitrary integer that does not decrease with
 *	release.  Stubs tables with different epochs are incompatible.
 *
 *----------------------------------------------------------------------
 */

int
TclBN_epoch(void)
{
    return TCLTOMMATH_EPOCH;
}

/*
 *----------------------------------------------------------------------
 *
 * TclBN_revision --
 *
 *	Returns the revision level of the TclTomMath stubs table
 *
 * Results:
 *	Returns an arbitrary integer that increases with revisions.
 *	If a client requires a given epoch and revision, any Stubs table
 *	with the same epoch and an equal or higher revision satisfies
 *	the request.
 *
 *----------------------------------------------------------------------
 */

int
TclBN_revision(void)
{
    return TCLTOMMATH_REVISION;
}
#if 0

/*
 *----------------------------------------------------------------------
 *
 * TclBNAlloc --
 *
 *	Allocate memory for libtommath.
 *
 * Results:
 *	Returns a pointer to the allocated block.
 *
 * This procedure is a wrapper around Tcl_Alloc, needed because of a
 * mismatched type signature between Tcl_Alloc and malloc.
 *
 *----------------------------------------------------------------------
 */

extern void *
TclBNAlloc(
    size_t x)
{
    return (void *) ckalloc((unsigned int) x);
}

/*
 *----------------------------------------------------------------------
 *
 * TclBNRealloc --
 *
 *	Change the size of an allocated block of memory in libtommath
 *
 * Results:
 *	Returns a pointer to the allocated block.
 *
 * This procedure is a wrapper around Tcl_Realloc, needed because of a
 * mismatched type signature between Tcl_Realloc and realloc.
 *
 *----------------------------------------------------------------------
 */

void *
TclBNRealloc(
    void *p,
    size_t s)
{
    return (void *) ckrealloc((char *) p, (unsigned int) s);
}

/*
 *----------------------------------------------------------------------
 *
 * TclBNFree --
 *
 *	Free allocated memory in libtommath.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory is freed.
 *
 * This function is simply a wrapper around Tcl_Free, needed in libtommath
 * because of a type mismatch between free and Tcl_Free.
 *
 *----------------------------------------------------------------------
 */

extern void
TclBNFree(
    void *p)
{
    ckree((char *) p);
}
#endif

/*
 *----------------------------------------------------------------------
 *
 * TclInitBignumFromLong --
 *
 *	Allocate and initialize a 'bignum' from a native 'long'.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The 'bignum' is constructed.
 *
 *----------------------------------------------------------------------
 */

void
TclInitBignumFromLong(
    mp_int *a,
    long v)
{
    if (mp_init_size(a, (CHAR_BIT * sizeof(long) + DIGIT_BIT - 1) / DIGIT_BIT) != MP_OKAY) {
	Tcl_Panic("initialization failure in TclBNInitBignumFromLong");
    }
    if (v < (long)0) {
	mp_set_long_long(a, (Tcl_WideUInt)(-(Tcl_WideInt)v));
	mp_neg(a, a);
    } else {
	mp_set_long_long(a, (Tcl_WideUInt)v);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclBNInitBignumFromWideInt --
 *
 *	Allocate and initialize a 'bignum' from a Tcl_WideInt
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The 'bignum' is constructed.
 *
 *----------------------------------------------------------------------
 */

void
TclInitBignumFromWideInt(
    mp_int *a,			/* Bignum to initialize */
    Tcl_WideInt v)		/* Initial value */
{
	if (mp_init_size(a, (CHAR_BIT * sizeof(Tcl_WideUInt) + DIGIT_BIT - 1) / DIGIT_BIT) != MP_OKAY) {
		Tcl_Panic("initialization failure in TclBNInitBignumFromWideInt");
	}
    if (v < (Tcl_WideInt)0) {
	mp_set_long_long(a, (Tcl_WideUInt)(-v));
	mp_neg(a, a);
    } else {
	mp_set_long_long(a, (Tcl_WideUInt)v);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * TclBNInitBignumFromWideUInt --
 *
 *	Allocate and initialize a 'bignum' from a Tcl_WideUInt
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	The 'bignum' is constructed.
 *
 *----------------------------------------------------------------------
 */

void
TclInitBignumFromWideUInt(
    mp_int *a,			/* Bignum to initialize */
    Tcl_WideUInt v)		/* Initial value */
{
	if (mp_init_size(a, (CHAR_BIT * sizeof(Tcl_WideUInt) + DIGIT_BIT - 1) / DIGIT_BIT) != MP_OKAY) {
	    Tcl_Panic("initialization failure in TclBNInitBignumFromWideUInt");
	}
	mp_set_long_long(a, v);
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
