# UnrealSequence

Sequence Wallet plugin for Unreal Engine

# Usage Instructions

## Install
TODO..?

## Setting up UnrealSequence
This setup assumes you're using UMG & Blueprints.

1. Create a Widget Blueprint. ![](ReadmeImages/CreateWidgetBlueprint.png "Widget Blueprint Right-Click Menu")
2. Select "WalletWidget" as the parent class. ![](ReadmeImages/PickWidgetParentClass.png "Widget Blueprint Parent Selector")
3. Give your blueprint a name, like `WBP_SequenceWallet`. ![](ReadmeImages/NameWidgetBlueprint.png "Widget Blueprint Naming")
4. Add the two required WebBrowser widgets to the blueprint. One must be named `DummyWebBrowser`, the other must be named `WalletWebBrowser`. The easiest way is to put them in a `CanvasPanel`. The `WalletWebBrowser` is the one that will render the Sequence Wallet, so you should probably make it full-size in your `CanvasPanel`. The `DummyWebBrowser` should probably be 0-sized & collapsed. ![](ReadmeImages/AddWebBrowserWidgets.png "Adding the Web Browser widgets")
5. If you'd like, you can now customize the default network and wallet app URL by changing the Wallet Widget settings under Details.![](ReadmeImages/EditDetails.png "Wallet Widget settings in the Details Panel")

## Hiding and showing the wallet
The Wallet blueprint you created will have events for when the wallet is requesting to be shown and hidden. You should hook those up in your UI to hide & show the blueprint.
![](ReadmeImages/HideShowBlueprint.png "Hiding and Showing the wallet when it requests.")

## Initialization
The wallet will be automatically initialized using the default network you configured in the widget settings.
You must wait for the OnSequenceInitialized event before using any Sequence functions.
![](ReadmeImages/OnSequenceInitialized.png "The OnSequenceInitialized function.")


## Connecting the Wallet
There is a `GetIsConnected` function you can call to see if the wallet is connected.
If it isn't, you can call the `Connect` function, and call `GetIsConnected` again afterwards to see if the user approved the connection.
![](ReadmeImages/ConnectWallet.png "Calling the Connect function.")

## Executing Sequence.js code, and getting callbacks

For Sequence.JS docs, see https://docs.sequence.xyz/

You can execute arbitrary Sequence.JS code with the function `Execute Sequence JS`. ![](ReadmeImages/ExecuteSequenceJS.png "Executing arbitrary Sequence JS.")
The Sequence SDK is available at the global object named `seq`. You can access the Wallet object with `seq.getWallet()`.

If you need to return data from a call, you can use the `Execute Sequence JS with Callback` function. In your JS code, call the function `cb` to return any arbitrary string to the Callback event you pass. ![](ReadmeImages/ExecuteSequenceJSWithCallback.png "Executing arbitrary Sequence JS with a Callback.")
If you don't call the `cb` function, you will leak memory.

### Important Note - Valid JS
ExecuteJS will collapse all your JS to one line. Make sure every statement is semicolon-terminated, and you don't have any `//` comments, only `/* */` comments.

## Console.log, warning, and errors
Logs from the browser contexts will show up in your Unreal log under the LogSequence category.