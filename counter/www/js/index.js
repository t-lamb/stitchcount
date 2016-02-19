var BUTTON_SERVICE = 'FBE0';
var COUNT_CHARACTERISTIC = 'FBE1';

var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
        this.showMainPage();
    },
    // Bind any events that are required on startup.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
        deviceList.addEventListener('click', this.connect, false);
        refreshButton.addEventListener('click', this.refreshDeviceList, false);
        disconnectButton.addEventListener('click', this.disconnect, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'refreshDeviceList'
    // function, we must explicitly call 'app.refreshDeviceList(...);'
    onDeviceReady: function() {
        FastClick.attach(document.body); // https://github.com/ftlabs/fastclick
        app.refreshDeviceList();
    },
    refreshDeviceList: function() {
        deviceList.innerHTML = ''; //empty the list
        ble.scan([BUTTON_SERVICE], 5, app.onDiscoverDevice, app.onError);
    },
    onDiscoverDevice: function(device) {
        var listItem = document.createElement('li');
        listItem.innerHTML = device.name + '<br/>' + 
            device.id + '<br/>' + 
            'RSSI: ' + device.rssi;
        listItem.dataset.deviceId = device.id;
        deviceList.appendChild(listItem);  
    },
    connect: function(e) {
        var deviceId = e.target.dataset.deviceId;
        ble.connect(deviceId, app.onConnect, app.onError);
    },
    onConnect: function(peripheral) {
        app.peripheral = peripheral;
        app.showDetailPage();

        var failure = function(peripheral) {
            navigator.notification.alert(reason, null, "Error!:(");
        };

        // subscribe to be notified when the button state changes
        ble.startNotification(
            peripheral.id,
            BUTTON_SERVICE,
            COUNT_CHARACTERISTIC,
            app.onCountChange,
            failure
        );

        //read the initial value
        ble.read(
            peripheral.id,
            BUTTON_SERVICE,
            COUNT_CHARACTERISTIC,
            app.onCountChange,
            failure
        );
    },
    showMainPage: function() {
        mainPage.hidden = false;
        detailPage.hidden = true;
    },
    showDetailPage: function() {
        mainPage.hidden = true;
        detailPage.hidden = false;
    },
    onCountChange: function(buffer) {
        var data = new Uint8Array(buffer);
        var count = data[0];
        var message = "<span>You've knit<br>" + count + " stitches</span>";
        stitchCount.innerHTML = message;
    },
    disconnect: function(e) {
        if (app.peripheral && app.peripheral.id) {
            ble.disconnect(app.peripheral.id, app.showMainPage, app.onError);
        }
    },
    onError: function(reason) {
        navigator.notification.alert(reason, app.showMainPage, 'Error');
    }
};

app.initialize();
