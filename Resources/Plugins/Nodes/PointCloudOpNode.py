from Katana import FnAttribute, Nodes3DAPI 

nb = Nodes3DAPI.NodeTypeBuilder('PointCloudOp')
nb.setInputPortNames(('in',))

nb.setParametersTemplateAttr(FnAttribute.GroupBuilder()
    .set('source', FnAttribute.StringAttribute(""))
    .set('dest', FnAttribute.StringAttribute(""))
    .set('instance', FnAttribute.StringAttribute(""))
    .set('instanceScale', FnAttribute.DoubleAttribute(0.01))
    .build())

nb.setHintsForNode({'help': 'Instantiates a point cloud with a piece of geometry'})
nb.setHintsForParameter('source', {'widget':'scenegraphLocation'})
nb.setHintsForParameter('dest', {'widget':'scenegraphLocation'})
nb.setHintsForParameter('instance', {'widget':'scenegraphLocation'})


def myBuildOps(node, interface):
    op = interface.buildOp('pointCloudOp')
    
    if op.areParametersDirty():
        frameTime = interface.getFrameTime()

        op.setOpType('PointCloudOp')
        op.setOpArg('source', node.getParameter('source').getValue(frameTime))
        op.setOpArg('dest', node.getParameter('dest').getValue(frameTime))
        op.setOpArg('instance', node.getParameter('instance').getValue(frameTime))
        op.setOpArg('instanceScale', FnAttribute.DoubleAttribute(node.getParameter('instanceScale').getValue(frameTime)))
    if op.areInputsDirty():
        op.setOpInputs(interface.getAllInputOps())

    return op

nb.setBuildOpFnc(myBuildOps)
nb.build()    # Registers the type