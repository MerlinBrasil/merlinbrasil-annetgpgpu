#include <gui/QScene.h>
#include <gui/QNode.h>
#include <gui/QEdge.h>
#include <gui/QLayer.h>
#include <gui/QLabel.h>
#include <gui/QZLabel.h>
#include <iostream>


Scene::Scene(QObject *parent) : QGraphicsScene(parent)
{
}

ANN::BPNet *Scene::getANNet() {

	m_pANNet = new ANN::BPNet;

	int LayerTypeFlag 	= -1;
	int iSize 			= -1;

	/**
	 * Create layers for neural net
	 */
	QList<ANN::BPLayer*> lLayers;
	foreach(Layer *pLayer, m_lLayers) {
		LayerTypeFlag = pLayer->getLabel()->getType();
		iSize = pLayer->nodes().size();

		assert(LayerTypeFlag > 0);
		assert(iSize > 0);

		int iZ = pLayer->getZLabel()->getZLayer();
		if(iZ < 0) {
			QMessageBox msgBox;
			msgBox.setText("Z-values must be set for all layers.");
			msgBox.exec();

			return NULL;
		}

		ANN::BPLayer *pBPLayer = new ANN::BPLayer(iSize, LayerTypeFlag);
		pBPLayer->SetZLayer(iZ);

		lLayers << pBPLayer;
	}

	/**
	 * Build connections
	 */
	std::vector<std::vector<int> > vIndices;
	foreach(Layer *pLayer, m_lLayers) {
		foreach(Node *pNode, pLayer->nodes() ) {

		}
	}

	/**
	 * Check z-layers
	 */

	/**
	 * Add layers to neural net
	 */
	foreach(ANN::BPLayer *pLayer, lLayers) {
		m_pANNet->AddLayer(pLayer);
	}

	return m_pANNet;
}

void Scene::setANNet(ANN::BPNet &) {

}

void Scene::adjust() {
    foreach (Edge *edge, m_lEdges)
        edge->adjust();
    foreach (Layer *layer, m_lLayers)
        layer->adjust();
}

Layer* Scene::addLayer(const unsigned int &iNodes, const QString &sName) {
    Layer *pLayer = new Layer;
    pLayer->setScene(this);

    for(unsigned int i = 0; i < iNodes; i++) {
        Node *pNode = new Node;
        pNode->setPos(i*(pNode->getWidth()+8), 0);
        pLayer->addNode(pNode);
        pNode->setLayer(pLayer);
        addItem(pNode);
    }
    addItem(pLayer);
    addItem(pLayer->addLabel(sName));
    addItem(pLayer->addZLabel(-1));
    m_lLayers << pLayer;
    pLayer->adjust();

    return pLayer;
}

void Scene::addNode(Node* pNode) {
    m_lNodes << pNode;
    addItem(pNode);
}

void Scene::addEdge(Edge* pEdge) {
    m_lEdges << pEdge;
    addItem(pEdge);
}

void Scene::removeEdge(Edge* pDelEdge) {
    removeItem(pDelEdge);

    QList<Edge*> pNewList;
    foreach(Edge *pEdge, m_lEdges) {
        if(pEdge != pDelEdge)
            pNewList << pEdge;
    }
    m_lEdges = pNewList;
}

void Scene::removeNode(Node* pDelNode) {
    removeItem(pDelNode);
    pDelNode->getLayer()->removeNode(pDelNode);

    QList<Node*> pNewList;
    foreach(Node *pNode, m_lNodes) {
        if(pNode != pDelNode)
            pNewList << pNode;
    }
    m_lNodes = pNewList;
}

void Scene::removeLayer(Layer* pDelLayer) {
    removeItem(pDelLayer->getLabel());
    removeItem(pDelLayer);

    foreach(Node *pNode, pDelLayer->nodes()) {
        removeNode(pNode);
    }

    QList<Layer*> pNewList;
    foreach(Layer *pLayer, m_lLayers) {
        if(pLayer != pDelLayer)
            pNewList << pLayer;
    }
    m_lLayers = pNewList;
}

QList<Edge*> Scene::edges() {
    return m_lEdges;
}

QList<Node*> Scene::nodes() {
    return m_lNodes;
}

QList<Layer*> Scene::layers() {
    return m_lLayers;
}