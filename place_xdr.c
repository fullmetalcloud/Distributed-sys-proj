/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "place.h"

bool_t
xdr_nametype (XDR *xdrs, nametype *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, MAXLEN))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_placelist (XDR *xdrs, placelist *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct place), (xdrproc_t) xdr_place))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_place (XDR *xdrs, place *objp)
{
	register int32_t *buf;

	 if (!xdr_nametype (xdrs, &objp->city))
		 return FALSE;
	 if (!xdr_nametype (xdrs, &objp->state))
		 return FALSE;
	 if (!xdr_nametype (xdrs, &objp->code))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_placelist (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_placename (XDR *xdrs, placename *objp)
{
	register int32_t *buf;

	 if (!xdr_nametype (xdrs, &objp->city))
		 return FALSE;
	 if (!xdr_nametype (xdrs, &objp->state))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_answer (XDR *xdrs, answer *objp)
{
	register int32_t *buf;

	 if (!xdr_placelist (xdrs, &objp->list))
		 return FALSE;
	 if (!xdr_nametype (xdrs, &objp->city))
		 return FALSE;
	 if (!xdr_nametype (xdrs, &objp->state))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->latitude))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->longitude))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->status))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_findplace_ret (XDR *xdrs, findplace_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->placeerr))
		 return FALSE;
	switch (objp->placeerr) {
	case 0:
		 if (!xdr_answer (xdrs, &objp->findplace_ret_u.ans))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
