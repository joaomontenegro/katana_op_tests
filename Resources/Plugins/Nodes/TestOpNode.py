from Katana import FnAttribute, Nodes3DAPI 

nb = Nodes3DAPI.NodeTypeBuilder('TestOp')
nb.setInputPortNames(('in',))

nb.setParametersTemplateAttr(FnAttribute.GroupBuilder()
    .set('argParent', FnAttribute.StringAttribute("father"))
    .set('argChild', FnAttribute.StringAttribute("son"))
    .build())

nb.setHintsForNode({'help': 'Node hints here!!!'})
nb.setHintsForParameter('attrName', {'help': 'Arg hints here!!!'})


def myBuildOps(node, interface):
    op = interface.buildOp('testParentOp')

    if op.areParametersDirty():
        frameTime = interface.getFrameTime()

        op.setOpType('TestParentOp')
        op.setOpArg('argParent', node.getParameter('argParent').getValue(frameTime))
        op.setOpArg('argChild', node.getParameter('argChild').getValue(frameTime))

    if op.areInputsDirty():
        op.setOpInputs(interface.getAllInputOps())

    return op

nb.setBuildOpFnc(myBuildOps)
nb.build()    # Registers the type