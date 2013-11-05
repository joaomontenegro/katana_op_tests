from Katana import QtGui, QtCore, QT4Widgets, UI4, Nodes3DAPI, ScenegraphManager, Utils, NodegraphAPI
from Nodes3DAPI.Manifest import FnAttribute, FnGeolib
from UI4.Tabs import BaseTab

ScenegraphManager = ScenegraphManager.Geolib3

class TestOpTab (BaseTab):
    def __init__(self, parent):
        BaseTab.__init__(self, parent)
        
        self.__runtime = None
        self.__client = None
        
        self.setLayout(QtGui.QVBoxLayout())
        self.layout().setSpacing(2)
        self.layout().setMargin(0)
        
        self.__locationsLabel = QtGui.QLabel('Locations')
        self.layout().addWidget(self.__locationsLabel)
        
        self.__locationInfoLabel = QtGui.QLabel('Info')
        self.layout().addWidget(self.__locationInfoLabel)
        
        self.__infoFrame = QtGui.QFrame()
        self.__infoFrame.setLayout(QtGui.QVBoxLayout())
        self.layout().addWidget(self.__infoFrame)
        
        self.__setLocations()
        self.__connectSignals()
        
        
    def __viewNodeChangedEvent(self, node):
        
        viewedTime = NodegraphAPI.GetCurrentTime()
        viewedNode = NodegraphAPI.GetViewNode()
        
        print "viewNode ---> ", viewedNode
        
        if not viewedNode:
            self.__runtime = None
            self.__client = None
            return
        
        
        self.__runtime = FnGeolib.GetRegisteredRuntimeInstance()
        txn = self.__runtime.createTransaction()
        
        op = Nodes3DAPI.GetOp(txn, viewedNode, viewedTime)
        
        if not op:
            self.__runtime = None
            return
        
        self.__client = txn.createClient()
        txn.setClientOp(self.__client, op)
        
        self.__runtime.commit(txn)
        
        self.__setLocations()
        
        
        
    def __connectSignals(self):
        Utils.EventModule.RegisterCollapsedHandler(self.__selectionChangedEvent,
                                                   "scenegraphManager_selectionChanged")
        
        Utils.EventModule.RegisterEventHandler(self.__idle, "event_idle", None)
        
        Utils.EventModule.RegisterCollapsedHandler(self.__viewNodeChangedEvent, 
                                                   "nodegraph_setCurrentTime")
        
        Utils.EventModule.RegisterCollapsedHandler(self.__viewNodeChangedEvent, 
                                                   'nodegraph_changed_affectsViewNodes')
        
        
    def __selectionChangedEvent(self, args):
        self.__setLocations()
        
        
    def __setLocations(self):
        sg = ScenegraphManager.getActiveScenegraph()
        
        if not sg:
            self.__locationsLabel.setText( "No ScenegraphManager" )
            return
                
        locations = sg.getSelectedLocations()
        if not locations:
            self.__locationsLabel.setText( "No Selection" )
            return
        
        self.__locationsLabel.setText( str(locations) )
        
        print "---active...>", locations
        if self.__client:
            self.__client.setLocationsActive(locations)
        
        
        
    def __idle(self, *args, **kwds):
        if not self.__client:
            return
        
        locationEvents = self.__client.getLocationEvents()
        
        if not locationEvents:
            return
 
        for event in locationEvents:
            text = ""
            if not event.hasLocationData():
                print "no data"
                continue
            
            locationPath = event.getLocationPath()
            
            text += locationPath + ":"
            
            print "--loc-> ", locationPath
            locationData = event.getLocationData()
            locationAttrs = locationData.getAttrs()
            
            if not isinstance(locationAttrs, FnAttribute.GroupAttribute):
                print "no attrs"
                continue
            
            typeAttr = locationAttrs.getChildByName("type")
            if not isinstance(typeAttr, FnAttribute.StringAttribute):
                print "no type"
                continue

            type = typeAttr.getNearestSample(0)[0]
            
            text += type + " : "
            
            geoAttr = locationAttrs.getChildByName("geometry")
            if not isinstance(geoAttr, FnAttribute.GroupAttribute):
                print "no geo"
                continue
            
            pointAttr = geoAttr.getChildByName("point")
            if not isinstance(pointAttr, FnAttribute.GroupAttribute):
                print "no point"
                continue
            
            pAttr = pointAttr.getChildByName("P")
            if not isinstance(pAttr, FnAttribute.FloatAttribute):
                print "no p"
                continue
            
            size = str(int(len(pAttr.getData()) / 3))
            
            text += " " + size + " points"
            
        self.__locationInfoLabel.setText(text)
            
            

        
        
        
        