/* sspgvx.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "pnl/pnl_f2c.h"

/* Table of constant values */

static int c__1 = 1;

 int sspgvx_(int *itype, char *jobz, char *range, char *
	uplo, int *n, float *ap, float *bp, float *vl, float *vu, int *il, 
	 int *iu, float *abstol, int *m, float *w, float *z__, int *
	ldz, float *work, int *iwork, int *ifail, int *info)
{
    /* System generated locals */
    int z_dim1, z_offset, i__1;

    /* Local variables */
    int j;
    extern int lsame_(char *, char *);
    char trans[1];
    int upper, wantz;
    extern  int stpmv_(char *, char *, char *, int *, 
	    float *, float *, int *), stpsv_(char *, 
	     char *, char *, int *, float *, float *, int *);
    int alleig, indeig, valeig;
    extern  int xerbla_(char *, int *), spptrf_(
	    char *, int *, float *, int *), sspgst_(int *, 
	    char *, int *, float *, float *, int *), sspevx_(
	    char *, char *, char *, int *, float *, float *, float *, 
	    int *, int *, float *, int *, float *, float *, int *
, float *, int *, int *, int *)
	    ;


/*  -- LAPACK driver routine (version 3.2) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
/*     November 2006 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SSPGVX computes selected eigenvalues, and optionally, eigenvectors */
/*  of a float generalized symmetric-definite eigenproblem, of the form */
/*  A*x=(lambda)*B*x,  A*Bx=(lambda)*x,  or B*A*x=(lambda)*x.  Here A */
/*  and B are assumed to be symmetric, stored in packed storage, and B */
/*  is also positive definite.  Eigenvalues and eigenvectors can be */
/*  selected by specifying either a range of values or a range of indices */
/*  for the desired eigenvalues. */

/*  Arguments */
/*  ========= */

/*  ITYPE   (input) INTEGER */
/*          Specifies the problem type to be solved: */
/*          = 1:  A*x = (lambda)*B*x */
/*          = 2:  A*B*x = (lambda)*x */
/*          = 3:  B*A*x = (lambda)*x */

/*  JOBZ    (input) CHARACTER*1 */
/*          = 'N':  Compute eigenvalues only; */
/*          = 'V':  Compute eigenvalues and eigenvectors. */

/*  RANGE   (input) CHARACTER*1 */
/*          = 'A': all eigenvalues will be found. */
/*          = 'V': all eigenvalues in the half-open interval (VL,VU] */
/*                 will be found. */
/*          = 'I': the IL-th through IU-th eigenvalues will be found. */

/*  UPLO    (input) CHARACTER*1 */
/*          = 'U':  Upper triangle of A and B are stored; */
/*          = 'L':  Lower triangle of A and B are stored. */

/*  N       (input) INTEGER */
/*          The order of the matrix pencil (A,B).  N >= 0. */

/*  AP      (input/output) REAL array, dimension (N*(N+1)/2) */
/*          On entry, the upper or lower triangle of the symmetric matrix */
/*          A, packed columnwise in a linear array.  The j-th column of A */
/*          is stored in the array AP as follows: */
/*          if UPLO = 'U', AP(i + (j-1)*j/2) = A(i,j) for 1<=i<=j; */
/*          if UPLO = 'L', AP(i + (j-1)*(2*n-j)/2) = A(i,j) for j<=i<=n. */

/*          On exit, the contents of AP are destroyed. */

/*  BP      (input/output) REAL array, dimension (N*(N+1)/2) */
/*          On entry, the upper or lower triangle of the symmetric matrix */
/*          B, packed columnwise in a linear array.  The j-th column of B */
/*          is stored in the array BP as follows: */
/*          if UPLO = 'U', BP(i + (j-1)*j/2) = B(i,j) for 1<=i<=j; */
/*          if UPLO = 'L', BP(i + (j-1)*(2*n-j)/2) = B(i,j) for j<=i<=n. */

/*          On exit, the triangular factor U or L from the Cholesky */
/*          factorization B = U**T*U or B = L*L**T, in the same storage */
/*          format as B. */

/*  VL      (input) REAL */
/*  VU      (input) REAL */
/*          If RANGE='V', the lower and upper bounds of the interval to */
/*          be searched for eigenvalues. VL < VU. */
/*          Not referenced if RANGE = 'A' or 'I'. */

/*  IL      (input) INTEGER */
/*  IU      (input) INTEGER */
/*          If RANGE='I', the indices (in ascending order) of the */
/*          smallest and largest eigenvalues to be returned. */
/*          1 <= IL <= IU <= N, if N > 0; IL = 1 and IU = 0 if N = 0. */
/*          Not referenced if RANGE = 'A' or 'V'. */

/*  ABSTOL  (input) REAL */
/*          The absolute error tolerance for the eigenvalues. */
/*          An approximate eigenvalue is accepted as converged */
/*          when it is determined to lie in an interval [a,b] */
/*          of width less than or equal to */

/*                  ABSTOL + EPS *   MAX( |a|,|b| ) , */

/*          where EPS is the machine precision.  If ABSTOL is less than */
/*          or equal to zero, then  EPS*|T|  will be used in its place, */
/*          where |T| is the 1-norm of the tridiagonal matrix obtained */
/*          by reducing A to tridiagonal form. */

/*          Eigenvalues will be computed most accurately when ABSTOL is */
/*          set to twice the underflow threshold 2*SLAMCH('S'), not zero. */
/*          If this routine returns with INFO>0, indicating that some */
/*          eigenvectors did not converge, try setting ABSTOL to */
/*          2*SLAMCH('S'). */

/*  M       (output) INTEGER */
/*          The total number of eigenvalues found.  0 <= M <= N. */
/*          If RANGE = 'A', M = N, and if RANGE = 'I', M = IU-IL+1. */

/*  W       (output) REAL array, dimension (N) */
/*          On normal exit, the first M elements contain the selected */
/*          eigenvalues in ascending order. */

/*  Z       (output) REAL array, dimension (LDZ, MAX(1,M)) */
/*          If JOBZ = 'N', then Z is not referenced. */
/*          If JOBZ = 'V', then if INFO = 0, the first M columns of Z */
/*          contain the orthonormal eigenvectors of the matrix A */
/*          corresponding to the selected eigenvalues, with the i-th */
/*          column of Z holding the eigenvector associated with W(i). */
/*          The eigenvectors are normalized as follows: */
/*          if ITYPE = 1 or 2, Z**T*B*Z = I; */
/*          if ITYPE = 3, Z**T*inv(B)*Z = I. */

/*          If an eigenvector fails to converge, then that column of Z */
/*          contains the latest approximation to the eigenvector, and the */
/*          index of the eigenvector is returned in IFAIL. */
/*          Note: the user must ensure that at least MAX(1,M) columns are */
/*          supplied in the array Z; if RANGE = 'V', the exact value of M */
/*          is not known in advance and an upper bound must be used. */

/*  LDZ     (input) INTEGER */
/*          The leading dimension of the array Z.  LDZ >= 1, and if */
/*          JOBZ = 'V', LDZ >= MAX(1,N). */

/*  WORK    (workspace) REAL array, dimension (8*N) */

/*  IWORK   (workspace) INTEGER array, dimension (5*N) */

/*  IFAIL   (output) INTEGER array, dimension (N) */
/*          If JOBZ = 'V', then if INFO = 0, the first M elements of */
/*          IFAIL are zero.  If INFO > 0, then IFAIL contains the */
/*          indices of the eigenvectors that failed to converge. */
/*          If JOBZ = 'N', then IFAIL is not referenced. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value */
/*          > 0:  SPPTRF or SSPEVX returned an error code: */
/*             <= N:  if INFO = i, SSPEVX failed to converge; */
/*                    i eigenvectors failed to converge.  Their indices */
/*                    are stored in array IFAIL. */
/*             > N:   if INFO = N + i, for 1 <= i <= N, then the leading */
/*                    minor of order i of B is not positive definite. */
/*                    The factorization of B could not be completed and */
/*                    no eigenvalues or eigenvectors were computed. */

/*  Further Details */
/*  =============== */

/*  Based on contributions by */
/*     Mark Fahey, Department of Mathematics, Univ. of Kentucky, USA */

/* ===================================================================== */

/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Test the input parameters. */

    /* Parameter adjustments */
    --ap;
    --bp;
    --w;
    z_dim1 = *ldz;
    z_offset = 1 + z_dim1;
    z__ -= z_offset;
    --work;
    --iwork;
    --ifail;

    /* Function Body */
    upper = lsame_(uplo, "U");
    wantz = lsame_(jobz, "V");
    alleig = lsame_(range, "A");
    valeig = lsame_(range, "V");
    indeig = lsame_(range, "I");

    *info = 0;
    if (*itype < 1 || *itype > 3) {
	*info = -1;
    } else if (! (wantz || lsame_(jobz, "N"))) {
	*info = -2;
    } else if (! (alleig || valeig || indeig)) {
	*info = -3;
    } else if (! (upper || lsame_(uplo, "L"))) {
	*info = -4;
    } else if (*n < 0) {
	*info = -5;
    } else {
	if (valeig) {
	    if (*n > 0 && *vu <= *vl) {
		*info = -9;
	    }
	} else if (indeig) {
	    if (*il < 1) {
		*info = -10;
	    } else if (*iu < MIN(*n,*il) || *iu > *n) {
		*info = -11;
	    }
	}
    }
    if (*info == 0) {
	if (*ldz < 1 || wantz && *ldz < *n) {
	    *info = -16;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("SSPGVX", &i__1);
	return 0;
    }

/*     Quick return if possible */

    *m = 0;
    if (*n == 0) {
	return 0;
    }

/*     Form a Cholesky factorization of B. */

    spptrf_(uplo, n, &bp[1], info);
    if (*info != 0) {
	*info = *n + *info;
	return 0;
    }

/*     Transform problem to standard eigenvalue problem and solve. */

    sspgst_(itype, uplo, n, &ap[1], &bp[1], info);
    sspevx_(jobz, range, uplo, n, &ap[1], vl, vu, il, iu, abstol, m, &w[1], &
	    z__[z_offset], ldz, &work[1], &iwork[1], &ifail[1], info);

    if (wantz) {

/*        Backtransform eigenvectors to the original problem. */

	if (*info > 0) {
	    *m = *info - 1;
	}
	if (*itype == 1 || *itype == 2) {

/*           For A*x=(lambda)*B*x and A*B*x=(lambda)*x; */
/*           backtransform eigenvectors: x = inv(L)'*y or inv(U)*y */

	    if (upper) {
		*(unsigned char *)trans = 'N';
	    } else {
		*(unsigned char *)trans = 'T';
	    }

	    i__1 = *m;
	    for (j = 1; j <= i__1; ++j) {
		stpsv_(uplo, trans, "Non-unit", n, &bp[1], &z__[j * z_dim1 + 
			1], &c__1);
/* L10: */
	    }

	} else if (*itype == 3) {

/*           For B*A*x=(lambda)*x; */
/*           backtransform eigenvectors: x = L*y or U'*y */

	    if (upper) {
		*(unsigned char *)trans = 'T';
	    } else {
		*(unsigned char *)trans = 'N';
	    }

	    i__1 = *m;
	    for (j = 1; j <= i__1; ++j) {
		stpmv_(uplo, trans, "Non-unit", n, &bp[1], &z__[j * z_dim1 + 
			1], &c__1);
/* L20: */
	    }
	}
    }

    return 0;

/*     End of SSPGVX */

} /* sspgvx_ */