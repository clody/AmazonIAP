This extension was built using Marmalade SDK v7.0.0 [353650] and Amazon In-App Purchase API in-app-purchasing-1.0.3.jar

This is just a simple extension that exposes a tiny part of the Amazon In-App Purchase API. (you can fork it and ad extra stuff if you need)
There is a method called AmazonIAPStartPurchase(char* productSKU) that will start the purchase process (only tested for CONSUMABLE and ENTITLEMENT products), and two Callbacks (AMAZONIAP_PURCHASE_SUCCESSFUL and AMAZONIAP_PURCHASE_FAILED) that will be called depending on the result of the transaction.


Usage
========================

- Include file:

```
  #include "AmazonIAP.h"
```


- First of all you should create the Callback functions:
```
  int32 AmazonIAPPurchaseSuccessfulCallback(void* system, void* user){
    char* productID = (char*) system;
    DoSomethingPurchseComplete(productSKU);
    delete productID;
    return 0;
  }

  int32 AmazonIAPPurchaseFailedCallback(void* system, void* user){
    char* productID = (char*) system;
    DoSomethingPurchseFailed(productSKU);
    delete productID;
    return 0;
}
```

- Then you need to register them when you init your app:
```
  if (AmazonIAPAvailable()) {
    AmazonIAPRegister(AMAZONIAP_PURCHASE_SUCCESSFUL, &AmazonIAPPurchaseSuccessfulCallback, NULL);
    AmazonIAPRegister(AMAZONIAP_PURCHASE_FAILED, &AmazonIAPPurchaseFailedCallback, NULL);
  }
```

- Now you can just call the method to start a purchase process:
```
  if (AmazonIAPAvailable()) {
    AmazonIAPStartPurchase(productSKU);
  }
```

- If the user completes the purchase or if he already had bought it before, the AMAZONIAP_PURCHASE_SUCCESSFUL callback will be called with the productSKU that initiaded the request. Otherwise, the AMAZONIAP_PURCHASE_FAILED callback will be called with the productSKU also.

