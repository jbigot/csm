import sys
import xml.dom.minidom
import xml.etree.ElementTree


useFastXmlImplem = True # False => Slow
indentXML = False # True => Very Slow


class InternalXmlImplem:
    class Element:
        def __init__(self, name, attr={}):
            self.name = name
            self.attr = attr
            self.tail = ''
            self.text = ''
            self.children = []
        def append(self, elem):
            self.children.append(elem)
        def str(self):
            if len(self.attr) > 0:
                attrStr = ' ' + ' '.join([k+'="'+v+'"' for k,v in self.attr.iteritems()])
            else:
                attrStr = ''
            if len(self.children) > 0:
                return '<' + self.name + attrStr + '>' + self.text + ''.join([e.str() for e in self.children]) + '</' + self.name + '>' + self.tail
            elif len(self.text) > 0:
                return '<' + self.name + attrStr + '>' + self.text + '</' + self.name + '>' + self.tail
            return '<' + self.name + attrStr + '/>' + self.tail

    class SubElement(Element):
        def __init__(self, parentNode, name, attr={}):
            self.name = name
            self.attr = attr
            self.tail = ''
            self.text = ''
            self.children = []
            parentNode.children.append(self)
    class Comment:
        def __init__(self, text):
            self.text = text
        def str(self):
            return '<!--' + self.text + '-->\n'
    @staticmethod
    def tostring(elem):
        return elem.str()


if useFastXmlImplem:
    xmlImplem = InternalXmlImplem
else:
    xmlImplem = xml.etree.ElementTree


def makeRootNode():
    root = xmlImplem.Element('lad', {'xmlns': 'http://www.inria.fr/avalon/lad'})
    child = xmlImplem.SubElement(root, 'mpi')
    return (root, child)


def makeProcess(parentNode):
    node = xmlImplem.SubElement(parentNode, 'process')
    #node.tail = '\n'
    #node.text = '\n'
    return node


def makeStartProperty(parentNode, instance, instanceProperty):
    node = xmlImplem.SubElement(parentNode, 'start_property', {'instance': instance, 'property': instanceProperty})
    #node.tail = '\n'
    return node


def makeInstance(parentNode, instanceId, instanceType):
    node = xmlImplem.SubElement(parentNode, 'instance', {'type': instanceType, 'id': instanceId})
    #node.tail = '\n'
    #node.text = '\n'
    return node


def makeConnection(parentNode, fromPort, toPorts):
    propNode = xmlImplem.SubElement(parentNode, 'property', {'id': fromPort})
    for toPort in toPorts:
        node = xmlImplem.SubElement(propNode, 'cppref', {'property': toPort[0], 'instance': toPort[1]})
        #node.tail = '\n'
    #propNode.tail = '\n'
    #propNode.text = '\n'
    return propNode


def makeValue(parentNode, propType, propId, propValue):
    propNode = xmlImplem.SubElement(parentNode, 'property', {'id': propId})
    #propNode.tail = '\n'
    propValueNode = xmlImplem.SubElement(propNode, 'value', {'type': propType})
    propValueNode.text = str(propValue)
    return propNode


def makeInt64(parentNode, propId, propValue):
    return makeValue(parentNode, 'uint64', propId, propValue)

def makeInt32(parentNode, propId, propValue):
    return makeValue(parentNode, 'uint32', propId, propValue)

def makeInt16(parentNode, propId, propValue):
    return makeValue(parentNode, 'uint16', propId, propValue)

def makeDouble(parentNode, propId, propValue):
    return makeValue(parentNode, 'double', propId, propValue)


def makeComment(parentNode, text):
    node = xmlImplem.Comment(text)
    #node.tail = '\n'
    parentNode.append(node)


def makeCommunicator(parentNode, peers):
    communicator = xmlImplem.SubElement(parentNode, 'communicator')
    for peer in peers:
        node = xmlImplem.SubElement(communicator, 'peer', {'instance':peer[0], 'property':peer[1]})
        #node.tail = '\n'
    #communicator.tail = '\n'
    #communicator.text = '\n'
    return communicator


def xmlToString(node):
    xmlString = xmlImplem.tostring(node)
    if indentXML:
        return xml.dom.minidom.parseString(xmlString.replace('\n', '')).toprettyxml(indent='    ')
    return xmlString

