/*
java implementation of the AmazonIAP extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
 

import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;

import android.util.Log;
import android.app.Activity;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.GetUserIdResponse.GetUserIdRequestStatus;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.ItemDataResponse.ItemDataRequestStatus;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchaseResponse.PurchaseRequestStatus;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;
import com.amazon.inapp.purchasing.PurchasingManager;
import com.amazon.inapp.purchasing.Receipt;

class AmazonIAP
{
    public native void AmazonIAPPurchaseSuccessfulCallback(String productID);
    public native void AmazonIAPPurchaseFailedCallback(String productID);
    
    MyPurchasingObserver purchasingObserver;
    
    AmazonIAP() {
        Log.i("AmazonIAP", "___________________________ registerObserver");
        purchasingObserver = new MyPurchasingObserver(LoaderActivity.m_Activity);
        PurchasingManager.registerObserver(purchasingObserver);
    }

    public int AmazonIAPStartPurchase(String productID)
    {
        Log.i("AmazonIAP", "___________________________ initiatePurchaseRequest, productID: " + productID);
        purchasingObserver.lastUsedProductID = productID;
        String requestId = PurchasingManager.initiatePurchaseRequest(productID);
        return 0;
    }
    
    private class MyPurchasingObserver extends BasePurchasingObserver {
         public String lastUsedProductID = null;
 
         public MyPurchasingObserver(Activity iapActivity) {
            super(iapActivity);
         }
         
         public void onSdkAvailable(final boolean isSandboxMode) {
            if(isSandboxMode) {
              Log.i("AmazonIAP", "___________________________ onSdkAvailable isSandboxMode true");
              Log.i("AmazonIAP", "___________________________ make sure to install AmazonSDKTester.apk on the device and create the amazon.sdktester.json file");
            } else {
              Log.i("AmazonIAP", "___________________________ onSdkAvailable isSandboxMode false");
            }
         }
         
         public void onGetUserIdResponse(final GetUserIdResponse response) {}
         
         public void onItemDataResponse(final ItemDataResponse response) {}
         
         public void onPurchaseResponse(final PurchaseResponse response) {
            Log.i("AmazonIAP", "___________________________ onPurchaseResponse ");
            PurchaseRequestStatus status = response.getPurchaseRequestStatus();

            if (status == PurchaseRequestStatus.SUCCESSFUL) {
                Log.i("AmazonIAP", "___________________________ status SUCCESSFUL");
                Receipt receipt = response.getReceipt();
                String sku = receipt.getSku();
                Log.i("AmazonIAP", "___________________________ sku: " + sku);
                AmazonIAPPurchaseSuccessfulCallback(sku);
            }
            
            if (status == PurchaseRequestStatus.ALREADY_ENTITLED) {
                Log.i("AmazonIAP", "___________________________ status ALREADY_ENTITLED");
                Log.i("AmazonIAP", "___________________________ lastUsedProductID: " + lastUsedProductID);
                AmazonIAPPurchaseSuccessfulCallback(lastUsedProductID);
            }

            if (status == PurchaseRequestStatus.FAILED || status == PurchaseRequestStatus.INVALID_SKU) {
                Log.i("AmazonIAP", "___________________________ status FAILED, lastUsedProductID: " + lastUsedProductID);
                AmazonIAPPurchaseFailedCallback(lastUsedProductID);
            }             
         }
         
         public void onPurchaseUpdatesResponse(final PurchaseUpdatesResponse response) {}
    }
}
