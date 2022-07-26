# UnrealSequence

Sequence Wallet plugin for Unreal Engine

# Usage Instructions

## Install
TODO..?

## Setting up UnrealSequence
This setup assumes you're using UMG & Blueprints.

1. Create a Widget Blueprint. ![Create Widget Blueprint](ReadmeImages/CreateWidgetBlueprint.png "Widget Blueprint Right-Click Menu")
2. Select "WalletWidget" as the parent class. ![Select Widget Blueprint Parent](ReadmeImages/PickWidgetParentClass.png "Widget Blueprint Parent Selector")
3. Give your blueprint a name, like `WBP_SequenceWallet`. ![Name Widget Blueprint](ReadmeImages/NameWidgetBlueprint.png "Widget Blueprint Naming")
4. Add the two required WebBrowser widgets to the blueprint. One must be named `DummyWebBrowser`, the other must be named `WalletWebBrowser`. The easiest way is to put them in a `CanvasPanel`. The `WalletWebBrowser` is the one that will render the Sequence Wallet, so you should probably make it full-size in your `CanvasPanel`. The `DummyWebBrowser` should probably be 0-sized or invisible, unless you're using it to debug Sequence.js code. ![Add Web Browser Widgets](ReadmeImages/AddWebBrowserWidgets.png "Adding the Web Browser widgets")
5. If you'd like, you can now customize the default network and wallet app URL by switching to Graph Editing Mode ![Graph Editing Selector](ReadmeImages/GoToGraphMode.png "Switching to Graph Editing"), and changing the Wallet Widget settings under Details.![Details Panel](ReadmeImages/EditDetails.png "Wallet Widget settings in the Details Panel")
6. Call IsConnected and Connect from your blueprints!

## Console.log, warning, and errors.
Logs from the browser contexts will show up in your Unreal log under the LogSequence category.