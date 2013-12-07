/*
Generic implementation of the AmazonIAP extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "AmazonIAP_internal.h"
s3eResult AmazonIAPInit()
{
    //Add any generic initialisation code here
    return AmazonIAPInit_platform();
}

void AmazonIAPTerminate()
{
    //Add any generic termination code here
    AmazonIAPTerminate_platform();
}

s3eResult AmazonIAPStartPurchase(const char* productID)
{
	return AmazonIAPStartPurchase_platform(productID);
}
