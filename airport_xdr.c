/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "airport.h"

bool_t
xdr_namelen (XDR *xdrs, namelen *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, MAXLENGTH))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airportlist (XDR *xdrs, airportlist *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)objp, sizeof (struct airport), (xdrproc_t) xdr_airport))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airport (XDR *xdrs, airport *objp)
{
	register int32_t *buf;

	 if (!xdr_namelen (xdrs, &objp->city))
		 return FALSE;
	 if (!xdr_namelen (xdrs, &objp->state))
		 return FALSE;
	 if (!xdr_namelen (xdrs, &objp->code))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->distance))
		 return FALSE;
	 if (!xdr_airportlist (xdrs, &objp->next))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_airportlocation (XDR *xdrs, airportlocation *objp)
{
	register int32_t *buf;

	 if (!xdr_float (xdrs, &objp->latitude))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->longitude))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_findairport_ret (XDR *xdrs, findairport_ret *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->airporterr))
		 return FALSE;
	switch (objp->airporterr) {
	case 0:
		 if (!xdr_airportlist (xdrs, &objp->findairport_ret_u.list))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
