#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ROS_INFO("Create Positioning object...");
    std::srand(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
    ui->setupUi(this);
    
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->customPlot->xAxis->setRange(-8, 8);
    ui->customPlot->yAxis->setRange(-5, 5);
    ui->customPlot->axisRect()->setupFullAxesBox();
    
    ui->customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->customPlot, "Interaction Example", QFont("sans", 17, QFont::Bold));
    ui->customPlot->plotLayout()->addElement(0, 0, title);
    
    ui->customPlot->xAxis->setLabel("x Axis");
    ui->customPlot->yAxis->setLabel("y Axis");
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
    updateGraph();
    ui->customPlot->rescaleAxes();
    
    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    
    // make bottom and left axes transfer their ranges to top and right axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    
    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
    
    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
    
    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}

MainWindow::~MainWindow()
{
  ROS_INFO("Cleaning up ...");
  
  // m_callbacks의 각 요소에 대한 메모리 할당 반환
  for(auto &cb : m_callbacks) {
      delete cb;
  }
  m_callbacks.clear();  // 옵션: 리스트를 비운다
  
  UWBpos6_terminate();
  delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.
   
   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */
  
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::updateGraph()
{
    // Clear previous graphs
    ui->customPlot->clearGraphs();

    // 태그의 위치를 추출하고, 그래프에 점으로 표현
    QVector<double> xPoints(4), yPoints(4);
    for (int i = 0; i < 4; i++) {
        xPoints[i] = pos.tag_pos_est[i].re;
        yPoints[i] = pos.tag_pos_est[i].im;
    }

    // 중심점 계산
    double xCenter = (xPoints[0] + xPoints[1] + xPoints[2] + xPoints[3]) / 4;
    double yCenter = (yPoints[0] + yPoints[1] + yPoints[2] + yPoints[3]) / 4;

    // 화살표의 끝점 계산
    double arrowLength = 0.5;  // 화살표의 적절한 길이를 선택
    double xArrowEnd = xCenter + arrowLength * cos(pos.heading_est);
    double yArrowEnd = yCenter + arrowLength * sin(pos.heading_est);

    // 네 개의 태그 위치에 점 표시
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(xPoints, yPoints);
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsNone);

    // Instead of adding an arrow item, create a new graph to represent the arrow
    QVector<double> arrowX(2), arrowY(2);
    arrowX[0] = xCenter;
    arrowY[0] = yCenter;
    arrowX[1] = xArrowEnd;
    arrowY[1] = yArrowEnd;

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(arrowX, arrowY);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    QPen arrowPen;
    arrowPen.setWidth(2); // Set to a suitable width
    arrowPen.setColor(Qt::red); // Or any color you prefer
    ui->customPlot->graph()->setPen(arrowPen);

    double maxDist = 0;
    for(int i = 0; i < 4; i++) {
        double dist = std::sqrt(std::pow(xPoints[i] - xCenter, 2) + std::pow(yPoints[i] - yCenter, 2));
        if(dist > maxDist) {
            maxDist = dist;
        }
    }

    double ratio = 1.0;  // 1:1 aspect ratio
    double range = 40;  // Range for both x and y axes
    double pixelWidth = ui->customPlot->xAxis->pixelToCoord(1) - ui->customPlot->xAxis->pixelToCoord(0);
    double pixelHeight = ui->customPlot->yAxis->pixelToCoord(0) - ui->customPlot->yAxis->pixelToCoord(1);

    ui->customPlot->yAxis->setScaleRatio(ui->customPlot->xAxis, pixelHeight / pixelWidth * ratio);

    // Set range
    ui->customPlot->xAxis->setRange(-range, range);
    ui->customPlot->yAxis->setRange(-range, range);


// ... [rest of your code]


    // 그래프 다시 그리기
    ui->customPlot->replot();
}



void MainWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(updateGraph()));
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }
  
  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}

void MainWindow::spinFor()
{
    RosKapDataPacket earliestPacket;
    UwbSubscriber* correspondingSubscriber = nullptr;  // <-- Add this line to remember the subscriber
    ROS_INFO("Start spinFor...");
    
    for (auto &cb : m_callbacks)
    {
        if (!cb->getDataEmpty())
        {
            RosKapDataPacket rosPacket = cb->m_kapCallback->next();
            
            // 첫 번째 패킷 또는 이전에 발견된 패킷보다 더 이른 패킷을 찾는 경우
            double rostimeValue = convertToDouble(rosPacket.first);
            double earliesttimeValue = convertToDouble(earliestPacket.first);
            if (earliestPacket.second.empty() || rostimeValue < earliesttimeValue)
            {
                earliestPacket = rosPacket;
                correspondingSubscriber = cb;  // <-- Update the subscriber
            }
        }
    }

    // 최소 타임스탬프 패킷을 찾은 경우 Operator 함수에 전달하고 pop
    if (!earliestPacket.second.empty())
    {
        Operator op(*this, earliestPacket.second, earliestPacket.first, pos); // MainWindow 참조 추가
        
        // TODO: pop 함수를 호출하여 earliestPacket 삭제
        if(correspondingSubscriber) {
            correspondingSubscriber->m_kapCallback->pop(earliestPacket);
        }
    }
}

double MainWindow::convertToDouble(const ros::Time& time) {
    return static_cast<double>(time.sec) + static_cast<double>(time.nsec) / 1e9;
}

void MainWindow::registerSubcribers(ros::NodeHandle &node) {
    bool should_publish;

    if (ros::param::get("/run_node/sub_UWB0", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "0"));
    }
    if (ros::param::get("/run_node/sub_UWB1", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "1"));
    }
    if (ros::param::get("/run_node/sub_UWB2", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "2"));
    }
    if (ros::param::get("/run_node/sub_UWB3", should_publish) && should_publish) {
        registerCallback(new UwbSubscriber(node, "3"));
    }
    // ... (repeat for other UWB nodes)
}

void MainWindow::registerCallback(UwbSubscriber *cb)  // Make sure PacketCallback is defined
{
    m_callbacks.push_back(cb);

    std::string callbacks_info = "Registered Callbacks: ";
    for (const auto& callback : m_callbacks) {
        callbacks_info += "[" + callback->getName() + "] ";  // Assuming you have a `getName()` function in your UwbSubscriber class
    }
    ROS_INFO("%s", callbacks_info.c_str());
}

bool MainWindow::handleError(std::string error)
{
    ROS_ERROR("%s", error.c_str());
    return false;
}
