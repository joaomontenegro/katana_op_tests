from Katana import FnAttribute, Nodes3DAPI 

nb = Nodes3DAPI.NodeTypeBuilder('PonyfierOp')
nb.setInputPortNames(('in',))

nb.setParametersTemplateAttr(FnAttribute.GroupBuilder()
    .set('cel', FnAttribute.StringAttribute(""))
    .build())

nb.setHintsForNode({'help': 'Node hints here!!!'})
nb.setHintsForParameter('cel', {'widget':'cel'})


def myBuildOps(node, interface):
    op = interface.buildOp('ponyfierOp')

    if op.areParametersDirty():
        frameTime = interface.getFrameTime()

        op.setOpType('PonyfierOp')
        op.setOpArg('cel', node.getParameter('cel').getValue(frameTime))

    if op.areInputsDirty():
        op.setOpInputs(interface.getAllInputOps())

    return op

nb.setBuildOpFnc(myBuildOps)
nb.build()    # Registers the type