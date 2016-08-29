<img src="data/logo.png"/>

The goal of quick native is to provide native user experience in components that require a fine integration in both Android and IOS. Also, this library is inspired by how [RectNative](http://www.reactnative.com) extensions work which briefly means it provides an interface that is platform independent.

One the main reasons to use QML/Qt is the fact that apps don't not need to run in their target platform all the time (through emulators or physical devices) instead, they can be executed locally which makes the process of writing mobile apps very convenient, therefore this library fallbacks to Qt's standard components when the running platform does not have a native one.

### Native Components
Component | Native Android | Native IOS |
--- | --- | ---
FileDialog | Done | Supported by default
TextInput | *upcoming* | *upcoming*

### Installation

This library is available through [npm](https://www.qpm.io):

    npm install -g quicknative


### Licensing
QuickNative is free software; you can redistribute it and/or modify it under the terms of the MIT License

