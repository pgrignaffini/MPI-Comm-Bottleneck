/***************************************************************
QGVCore Sample
Copyright (c) 2014, Bergont Nicolas, All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.
***************************************************************/
#include "MainWindow.h"
#include "moc_MainWindow.cpp"
#include "ui_MainWindow.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include "JsonParser.h"
#include <QMessageBox>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _scene = new QGVScene("Network", this);
    ui->graphicsView->setScene(_scene);

    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawGraph()
{

    std::vector<QGVNode*> nodes;
    std::vector<int> entries;
    JsonParser parser("dataFinal3.json");

    int** recv_size = parser.parseMatrix("m_recv_size");
    std::cout << "M_recv_size:" << std::endl;
    parser.printMatrix(recv_size);
    int** recv_count = parser.parseMatrix("m_recv_count");
    std::cout << "M_recv_count:" << std::endl;
    parser.printMatrix(recv_count);
    int** send_size = parser.parseMatrix("m_send_size");
    std::cout << "M_send_size:" << std::endl;
    parser.printMatrix(send_size);
    int** send_count = parser.parseMatrix("m_send_count");
    std::cout << "M_send_count:" << std::endl;
    parser.printMatrix(send_count);


    //Configure scene attributes
   // _scene->setGraphAttribute("label", "DEMO");

    _scene->setGraphAttribute("splines", "line");
    _scene->setGraphAttribute("rankdir", "LR");
    _scene->setGraphAttribute("ranksep", "2");
    _scene->setGraphAttribute("nodesep", "0.4");
    _scene->setGraphAttribute("layout", "circo");

    _scene->setNodeAttribute("shape", "box");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    _scene->setNodeAttribute("height", "1.2");
    _scene->setEdgeAttribute("minlen", "3");
    //_scene->setEdgeAttribute("dir", "both");

    int size = parser.getSize();

    for (int i = 0; i < size; i++)
    {
        QString node_name = "NODE";
        node_name.append(QString::number(i));
        QGVNode* node = _scene->addNode(node_name);
        node->setIcon(QImage(":/web-design.png"));
        entries = parser.parseMatrixLine(recv_size, i);
        node->setRecv_size(entries);
        entries = parser.parseMatrixLine(recv_count, i);
        node->setRecv_count(entries);
        entries = parser.parseMatrixLine(send_size, i);
        node->setSend_size(entries);
        entries = parser.parseMatrixLine(send_count, i);
        node->setSend_count(entries);
        node->p_recv_err = parser.parseValue("p_recv_err", i);
        node->p_send_err = parser.parseValue("p_send_err", i);
        node->p_recv_size = parser.parseValue("p_recv_size", i);
        node->p_send_size = parser.parseValue("p_send_size", i);
        node->p_recv_count = parser.parseValue("p_recv_count", i);
        node->p_send_count = parser.parseValue("p_send_count", i);
        nodes.push_back(node);
    }

    QString color;
    double threshold_max;
    double threshold_min;
    int max = 0;
    int min = 10000;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i!=j)
            {
                int count = send_size[i][j] * send_count[i][j];
                if (count > max) max = count;
                if (count < min) min = count;

            }
        }
    }

    threshold_max = (max - min) * (4.0/5.0);
    threshold_min = (max - min) * (1.0/5.0);

    /*
    std::cout << "max: " << max << std::endl;
    std::cout << "min: " << min << std::endl;
    std::cout << "Th max: " << threshold_max << std::endl;
    std::cout << "Th min: " << threshold_min << std::endl;*/

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (i!=j)
            {
                int count = send_size[i][j] * send_count[i][j];
                if (count > threshold_max) color = "red";
                else if (count < threshold_min) color = "green";
                else    color = "orange";

                _scene->addEdge(nodes[i], nodes[j], "")->setAttribute("color", color);
            }
        }
    }

    //std::cout<< pkt_sent[i][j] << std::endl;

    //Layout scene
    _scene->applyLayout();

    //Fit in view
    ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::nodeContextMenu(QGVNode *node)
{
    /*
    //Context menu exemple
    QMenu menu(node->label());

    menu.addSeparator();
    menu.addAction(tr("Informations"));
    menu.addAction(tr("Options"));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;*/
}

void MainWindow::nodeDoubleClick(QGVNode *node)
{
    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1\nM_Recv size: %2\nM_Recv count: %3\nM_Send size: %4\nM_Send count: %5\nP_Recv count: %6\nP_Send count: %7\nP_Recv size: %8\nP_Send size: %9\nP_Recv err: %10\nP_Send err: %11").arg(node->label()).arg(QString::number(node->getRecv_size())).arg(QString::number(node->getRecv_count())).arg(QString::number(node->getSend_size())).arg(QString::number(node->getSend_count())).arg(QString::number(node->p_recv_count)).arg(QString::number(node->p_send_count)).arg(QString::number(node->p_recv_size)).arg(QString::number(node->p_send_size)).arg(QString::number(node->p_recv_err)).arg(QString::number(node->p_send_err)));
}
