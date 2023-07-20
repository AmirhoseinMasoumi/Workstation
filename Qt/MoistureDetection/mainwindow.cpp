#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialImages();

    if(calibrated == false){
        ui->dm_Normal->setChecked(false);
        ui->dm_Normal->setCheckable(false);
        ui->dm_Normal->setEnabled(false);
        ui->dm_Calibration->setChecked(true);
    }

    ui->Calirbate_btn->setEnabled(false);

    sheet_coefficient = 10;
}

MainWindow::~MainWindow()
{
    if(connectionStatus == true){
        socket->close();
    }
    delete ui;
}

void MainWindow::initialImages()
{
    QColor background_color;
    background_color.setRed(240);
    background_color.setBlue(240);
    background_color.setGreen(240);

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Right_colorMatrix[i][j] = Qt::red; // First box
            Left_colorMatrix[i][j] = Qt::red;  // First box
            Clear_colorMatrix[i][j] = Qt::red;
        }
    }
    createRectangleGrid(ui->Right_Image, Right_colorMatrix);
    createTemperatureRect(ui->Right_Image, 750, 80);
    createRectangleGrid(ui->left_Image, Left_colorMatrix);
    createTemperatureRect(ui->left_Image, 750, 80);
}

void MainWindow::readValues(QList<int> vals)
{
    QColor background_color;
    background_color.setRed(240);
    background_color.setBlue(240);
    background_color.setGreen(240);

//    qDebug() << vals;
//    qDebug() << "****************";

    QList<int> newVals;
    foreach (int val, vals) {
        if(val == 0){
            val = 200000;
            newVals.append(val);
        }
        else{
           newVals.append(val);
        }
    }


//    for (int i = 400; i < newVals.size(); ++i) {
//        newVals[i] *= 1.05;
//    }

    for(int i = 0; i < newVals.size(); i++){
        if(newVals.at(i) < data_L.at(i)){
            newVals[i] = data_L.at(i);
        }
        else if(newVals.at(i) > data_H.at(i)){
            newVals[i] = data_H.at(i);
        }
    }
    QList<int> Moisture;
    for(int i = 0; i < 800; i++){
        float newVal = static_cast<float>(newVals.at(i)-data_L.at(i)) / static_cast<float>(data_H.at(i)-data_L.at(i)) * static_cast<float>(100 * static_cast<float>(10/sheet_coefficient));
        Moisture.append(newVal);
    }

//    qDebug() << Moisture;
//    qDebug() << "------33--------";

    /////////////////////////////////////////////////////////////////////////////////////
    int mVals[ROWS][COLS];
    int mVals2[ROWS][COLS];
    int cnt = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mVals[i][j] = Moisture.at(cnt++);
        }
    }
    for (int i = 1; i < ROWS-1; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mVals2[i][j] = (mVals[i][j] + mVals[i-1][j] + mVals[i+1][j])/3;
        }
    }

    cnt = 0;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Moisture[cnt++] = mVals2[i][j];
        }
    }
    //////////////
    cnt = 400;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mVals[i][j] = Moisture.at(cnt++);
        }
    }
    for (int i = 1; i < ROWS-1; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mVals2[i][j] = (mVals[i][j] + mVals[i-1][j] + mVals[i+1][j])/3;
        }
    }
    cnt = 400;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            Moisture[cnt++] = mVals2[i][j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        Moisture[i] = Moisture[i+16];
    }
    for (int i = 383; i < 400; ++i) {
        Moisture[i] = Moisture[i-16];
    }
    for (int i = 400; i < 416; ++i) {
        Moisture[i] = Moisture[i+16];
    }
    for (int i = 783; i < 800; ++i) {
        Moisture[i] = Moisture[i-16];
    }

    //------------clear check
//    clearCheck(Moisture);
//    blueCheck(Moisture);
    /////////////////////////////////////////////////////////////////////////////////////

    qDebug() << Moisture;
    qDebug() << "--------------";

    int counter_r = 0;
    int counter_l = 400;
    if(calibration == true){
        for (int i = 0; i < ROWS; ++i) {
            Right_colorMatrix[i][15] = getColor(100, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][14] = getColor(100, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][13] = getColor(Moisture[counter_r-1], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][12] = getColor(Moisture[counter_r-1], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][11] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][10] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][9] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][8] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][7] = getColor(Moisture[counter_r]-10, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][6] = getColor(Moisture[counter_r]-10, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][5] = getColor(Moisture[counter_r]-10, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][4] = getColor(Moisture[counter_r]-10, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][3] = getColor(Moisture[counter_r-1]-10, 0, 100);counter_r++; // First box
            Right_colorMatrix[i][2] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][1] = getColor(Moisture[counter_r+1], 0, 100);counter_r++; // First box
            Right_colorMatrix[i][0] = getColor(Moisture[counter_r], 0, 100);counter_r++; // First box


            Left_colorMatrix[i][0] = getColor(100, 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][1] = getColor(100, 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][2] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][3] = getColor(Moisture[counter_l+1], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][4] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][5] = getColor(Moisture[counter_l-1], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][6] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][7] = getColor(Moisture[counter_l-1], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][8] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][9] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][10] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][11] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][12] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][13] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][14] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
            Left_colorMatrix[i][15] = getColor(Moisture[counter_l], 0, 100);counter_l++;  // First box
        }

        createRectangleGrid(ui->Right_Image, Clear_colorMatrix);
        createRectangleGrid(ui->left_Image, Clear_colorMatrix);
        QTimer::singleShot(2000, [this]() {
            createRectangleGrid(ui->Right_Image, Right_colorMatrix);
            createRectangleGrid(ui->left_Image, Left_colorMatrix);
        });
//        createRectangleGrid(ui->Right_Image, Right_colorMatrix);
//        createRectangleGrid(ui->left_Image, Left_colorMatrix);
    }
    else{
        // calibration
    }

}

void MainWindow::createRectangleGrid(QGroupBox *groupBox, QColor colorMatrix[ROWS][COLS])
{
    QWidget *gridWidget = new QWidget(groupBox);
    gridWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(0);

    // Create the rectangles and add them to the grid layout
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            QWidget *rectWidget = new QWidget(gridWidget);
            rectWidget->setFixedSize(30, 30);
            rectWidget->setStyleSheet(QString("background-color: %1;").arg(colorMatrix[i][j].name()));

            QPalette palette = rectWidget->palette();
            palette.setColor(rectWidget->backgroundRole(), colorMatrix[i][j]);
            rectWidget->setPalette(palette);

            QGridLayout *rectLayout = new QGridLayout(rectWidget);
            rectLayout->setSpacing(0);
            rectLayout->setContentsMargins(0, 0, 0, 0);

            gridLayout->addWidget(rectWidget, i, j);
        }
    }

    // Clear the existing layout of the group box before setting the new one
    QLayout* existingLayout = groupBox->layout();
    if (existingLayout) {
        delete existingLayout;
    }

    groupBox->setLayout(new QHBoxLayout);
    groupBox->layout()->addWidget(gridWidget);
}

void MainWindow::createTemperatureRect(QGroupBox *groupBox, int x, int y)
{
    // Define the gradient colors
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, 800));
//    gradient.setColorAt(1.0, Qt::blue);
//    gradient.setColorAt(0.3, Qt::yellow);
//    gradient.setColorAt(0.0, Qt::red);

    QColor lightblue;
    lightblue.setRed(52);
    lightblue.setBlue(235);
    lightblue.setGreen(192);

    gradient.setColorAt(1.0, Qt::blue);
    gradient.setColorAt(0.5, lightblue);
    gradient.setColorAt(0.3, Qt::yellow);
    gradient.setColorAt(0.1, Qt::yellow);
    gradient.setColorAt(0.0, Qt::red);

    // Create the temperature rectangle
    QLabel *rectLabel = new QLabel(groupBox);
    rectLabel->setFixedSize(80, 800);
    rectLabel->move(x, y);

    // Create the pixmap and painter
    QPixmap pixmap(rectLabel->size());
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::NoPen);

    // Draw the temperature rectangle onto the pixmap
    painter.drawRect(rectLabel->rect());
    painter.end();

    // Set the pixmap as the background of the temperature rectangle label
    rectLabel->setPixmap(pixmap);

    // Set the mask of the temperature rectangle label to make it circular
    QPixmap maskPixmap(rectLabel->size());
    maskPixmap.fill(Qt::color0);
    QPainter maskPainter(&maskPixmap);
    maskPainter.setRenderHint(QPainter::Antialiasing);
    maskPainter.setBrush(QBrush(Qt::color1));
    maskPainter.setPen(Qt::NoPen);
    maskPainter.drawEllipse(rectLabel->rect());
    maskPainter.end();
}

QColor MainWindow::getColor(int val, double min1, double max1)
{
    // Create the gradient
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, 800));
    QColor lightblue;
    lightblue.setRed(52);
    lightblue.setGreen(192);
    lightblue.setBlue(235);

    QColor lightblue2;
    lightblue2.setRed(52);
    lightblue2.setGreen(82);
    lightblue2.setBlue(235);

    QColor orange;
    orange.setRed(240);
    orange.setGreen(54);
    orange.setBlue(7);

//    gradient.setColorAt(0.0, Qt::blue);
//    gradient.setColorAt(0.3, lightblue2);
//    gradient.setColorAt(0.6, Qt::yellow);
////    gradient.setColorAt(0.7, Qt::yellow);
//    gradient.setColorAt(0.8, orange);
//    gradient.setColorAt(1.0, Qt::red);

    gradient.setColorAt(0.0, Qt::blue);
    gradient.setColorAt(0.00000005, lightblue);
    gradient.setColorAt(0.15, lightblue);
//    gradient.setColorAt(0.35, Qt::green);
//    gradient.setColorAt(0.20, Qt::yellow);
    gradient.setColorAt(0.50, Qt::yellow);
    gradient.setColorAt(1.0, Qt::red);

//    gradient.setColorAt(1.0, Qt::blue);
//    gradient.setColorAt(0.3, lightblue);
//    gradient.setColorAt(0.2, Qt::yellow);
//    gradient.setColorAt(0.0, Qt::red);


    // Get the gradient stops
    QGradientStops stops = gradient.stops();

    // Map value to 0-1
    double min2 = 0; // Minimum value of target range
    double max2 = 1; // Maximum value of target range

    double mappedValue = (val - min1) * (max2 - min2) / (max1 - min1) + min2;

    // Interpolate between the gradient stops to get the color at the mapped value
    QColor color;
    for (int i = 1; i < stops.size(); i++) {
        if (stops[i].first >= mappedValue) {
            double t = (mappedValue - stops[i-1].first) / (stops[i].first - stops[i-1].first);
            color.setRedF(stops[i-1].second.redF() * (1-t) + stops[i].second.redF() * t);
            color.setGreenF(stops[i-1].second.greenF() * (1-t) + stops[i].second.greenF() * t);
            color.setBlueF(stops[i-1].second.blueF() * (1-t) + stops[i].second.blueF() * t);
            break;
        }
    }

    // Create an image from the gradient
    QImage image(1, 800, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setBrush(gradient);
    painter.drawRect(0, 0, 1, 800);

    //qDebug() << "Interpolated color: " << color;
    return color;
}

QColor MainWindow::getColorR(int val, double min1, double max1)
{
    // Create the gradient
    QLinearGradient gradient(QPoint(0, 0), QPoint(0, 800));
    QColor lightblue;
    lightblue.setRed(52);
    lightblue.setGreen(192);
    lightblue.setBlue(235);

    QColor lightblue2;
    lightblue2.setRed(52);
    lightblue2.setGreen(82);
    lightblue2.setBlue(235);

    QColor orange;
    orange.setRed(240);
    orange.setGreen(54);
    orange.setBlue(7);

//    gradient.setColorAt(0.0, Qt::blue);
//    gradient.setColorAt(0.15, lightblue);
//    gradient.setColorAt(0.25, lightblue);
//    gradient.setColorAt(0.35, Qt::green);
//    //    gradient.setColorAt(0.20, Qt::yellow);
//    gradient.setColorAt(0.45, Qt::yellow);
//    gradient.setColorAt(1.0, Qt::red);

    gradient.setColorAt(0.0, Qt::blue);
    gradient.setColorAt(0.5, lightblue);
    gradient.setColorAt(0.6, Qt::yellow);
    gradient.setColorAt(0.9, Qt::yellow);
    gradient.setColorAt(1.0, Qt::red);


    // Get the gradient stops
    QGradientStops stops = gradient.stops();

    // Map value to 0-1
    double min2 = 0; // Minimum value of target range
    double max2 = 1; // Maximum value of target range

    double mappedValue = (val - min1) * (max2 - min2) / (max1 - min1) + min2;

    // Interpolate between the gradient stops to get the color at the mapped value
    QColor color;
    for (int i = 1; i < stops.size(); i++) {
        if (stops[i].first >= mappedValue) {
            double t = (mappedValue - stops[i-1].first) / (stops[i].first - stops[i-1].first);
            color.setRedF(stops[i-1].second.redF() * (1-t) + stops[i].second.redF() * t);
            color.setGreenF(stops[i-1].second.greenF() * (1-t) + stops[i].second.greenF() * t);
            color.setBlueF(stops[i-1].second.blueF() * (1-t) + stops[i].second.blueF() * t);
            break;
        }
    }

    // Create an image from the gradient
    QImage image(1, 800, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setBrush(gradient);
    painter.drawRect(0, 0, 1, 800);

    //qDebug() << "Interpolated color: " << color;
    return color;
}

void MainWindow::processPendingDatagrams()
{
    int packetSize = 1200;
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress sender;
        uint16_t port;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

//        qDebug() << "Received data:" << datagram.size() << " : " << datagram;
        if(calibrated == true){
            if(datagram.size() == packetSize){
                if(datagram.count(header0) == packetSize || datagram.count(header1) == packetSize ||datagram.count(header2) == packetSize ||datagram.count(header3) == packetSize || datagram.count(header4) == packetSize){
                    //                qDebug() << "Header Received";
                    QString tab = "Tab: " + QString(datagram.at(1));
                    ui->tab_lbl->setText(tab);
                    readyWrite = true;
                    return;
                }
                if(datagram.count(footer) == packetSize){
                    //                qDebug() << "Footer Received";
                    readyWrite = false;

                    QList<QString> data_str_list = QString(packet).split('$');
                    packet.clear();

                    QList<int> data_list;
                    for (QString str : data_str_list) {
                        data_list.append(str.toInt());
                    }
                    data_list.removeLast();
                    if(data_list.size() == 800){
                        readValues(data_list);
//                        qDebug() << data_list;
//                        qDebug() << "----------------------";
                    }
                    else{
                        qDebug() << "Packet failed" << data_list.size();
//                        qDebug() << data_list;
                    }


                    return;
                }
                if(readyWrite == true){
                    //                qDebug() << "Data Received";
                    packet.append(datagram);
                    //                qDebug() << packet;
                    return;
                }
            }
        }
    }
}

bool MainWindow::checkCalFile()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString Cal_H = appDir + "/Cal_H.txt";
    QString Cal_L = appDir + "/Cal_L.txt";
    QFile file_H(Cal_H);
    QFile file_L(Cal_L);

    if (file_H.exists())
    {
        if (file_H.open(QIODevice::ReadWrite))
        {
            if (!file_H.atEnd())
            {
                QTextStream stream(&file_H);
                QString fileData = stream.readAll();

                QList<QString> data_str_list = QString(fileData).split('$');

                for (QString str : data_str_list) {
                    data_H.append(str.toInt());
//                    data_H.append(str.toInt() + 100);
//                    data_L.append(str.toInt() - 400);
                }
                if(data_H.count() == 800){
                    calibrated = true;
                    qDebug() << "Data_H is correct";

                    // test
                    for (int i = 0; i < 400; ++i) {
                        data_H[i] *= 1.05;
                    }
//                    for (int i = 400; i < 800; ++i) {
//                        data_H[i] += 500;
//                    }
                }
                else{
                    calibrated = false;
                }
            }
            else
            {
                qDebug() << "Data_H is empty.";
                calibrated = false;
            }
            file_H.close();
        }
        else
        {
            qDebug() << "Data_H file is empty.";
            calibrated = false;
        }
    }
    else{
        qDebug() << "Data_H file does not exist.";
        calibrated = false;
    }

    if (file_L.exists())
    {
        if (file_L.open(QIODevice::ReadWrite))
        {
            if (!file_L.atEnd())
            {
                QTextStream stream(&file_L);
                QString fileData = stream.readAll();

                QList<QString> data_str_list = QString(fileData).split('$');

                for (QString str : data_str_list) {
                    data_L.append(str.toInt()); // *0.9
                }
                if(data_L.count() == 800){
                    calibrated = true;
                    qDebug() << "Data_L is correct";

                    // test
                    for (int i = 0; i < 400; ++i) {
                        data_L[i] *= 1.00; // *1.05
                    }
                    for (int i = 400; i < 800; ++i) {
                        data_L[i] *= 1.00; // *1.01
                    }
                }
                else{
                    calibrated = false;
                }
            }
            else
            {
                qDebug() << "Data_L is empty.";
                calibrated = false;
            }
            file_L.close();
        }
        else
        {
            qDebug() << "Data_L file is empty.";
            calibrated = false;
        }
    }
    else{
        qDebug() << "Data_L file does not exist.";
        calibrated = false;
    }

    return calibrated;
}

////////////////////////////////////////////////////////////////

void MainWindow::on_coefficient_Custom_toggled(bool checked)
{
    if(checked){
        ui->coefficient_input->setEnabled(true);
        ui->select_btn->setEnabled(true);
    }else{
        ui->coefficient_input->setEnabled(false);
        ui->select_btn->setEnabled(false);
    }
}


void MainWindow::on_dm_Calibration_toggled(bool checked)
{
    calibration = checked;
    if(checked == true){
        ui->Calirbate_btn->setEnabled(true);
    }
    else{
        ui->Calirbate_btn->setEnabled(false);
    }
}


void MainWindow::on_coefficient_8_clicked()
{
    sheet_coefficient = 10;
}


void MainWindow::on_coefficient_10_clicked()
{
    sheet_coefficient = 12;
}


void MainWindow::on_coefficient_12_clicked()
{
    sheet_coefficient = 14;
}


void MainWindow::on_select_btn_clicked()
{
    sheet_coefficient = ui->coefficient_input->text().toInt();
}


void MainWindow::on_Calirbate_btn_clicked()
{
    if(connectionStatus == true){
        socket->close();
        bool ok;
        int port = ui->devicePort->text().toInt(&ok);
        QString ip = ui->deviceIp->text();
        Calibration *cal_dialog = new Calibration(this, ip, port);
        int result = cal_dialog->exec();
        if (result == QDialog::Accepted)
        {
            ui->dm_Calibration->setChecked(false);
            ui->dm_Normal->setChecked(true);

            connectionStatus = false;
            ui->connectBtn->setText("Connect");

            qDebug() << "Calibration finished";
        }
        else if (result == QDialog::Rejected)
        {
            qDebug() << "Calibration canceled";
        }

    }
}

void MainWindow::on_connectBtn_clicked()
{
    if(connectionStatus == false){
        calibrated = checkCalFile();
        qDebug() << "Calibration status: " << calibrated;
        if(calibrated == false){
            ui->dm_Normal->setChecked(false);
            ui->dm_Normal->setCheckable(false);
            ui->dm_Normal->setEnabled(false);
            ui->dm_Calibration->setChecked(true);
        }else{
            ui->dm_Normal->setCheckable(true);
            ui->dm_Normal->setEnabled(true);
            ui->dm_Calibration->setChecked(false);
            ui->dm_Normal->setChecked(true);
            ui->Calirbate_btn->setEnabled(false);
            ui->dm_Calibration->setEnabled(true);
        }
        if(ui->dm_Normal->isChecked()){
            ui->Calirbate_btn->setEnabled(false);
        }

        socket = new QUdpSocket(this);
        bool ok;
        int port = ui->devicePort->text().toInt(&ok);
        QString Ip = ui->deviceIp->text();
        if(ok){
//            bool result =  socket->bind(QHostAddress(Ip), port);
            bool result =  socket->bind(QHostAddress::AnyIPv4, port);
            if(result)
            {
                qDebug() << "Connected to port " + ui->devicePort->text();
                connectionStatus = true;
                ui->Calirbate_btn->setEnabled(true);
                ui->connectBtn->setText("Disconnect");
            }
            else
            {
                qDebug() << "Failed to connect to port " + ui->devicePort->text();
                connectionStatus = false;
                ui->Calirbate_btn->setEnabled(false);
                ui->connectBtn->setText("Connect");
            }
            connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()),Qt::AutoConnection);
        }
    }
    else{
        socket->close();
        connectionStatus = false;
        ui->connectBtn->setText("Connect");
        qDebug() << "Disconnected from port " + ui->devicePort->text();
        data_H.clear();
        data_L.clear();

        if(connectionStatus == true){
            ui->Calirbate_btn->setEnabled(true);
        }
        else{
            ui->Calirbate_btn->setEnabled(false);
            ui->dm_Normal->setEnabled(false);
            ui->dm_Calibration->setEnabled(false);
        }
    }
}


void MainWindow::on_dm_Normal_toggled(bool checked)
{
    calibration = checked;
    if(checked == false){
        ui->Calirbate_btn->setEnabled(true);
    }
    else{
        ui->Calirbate_btn->setEnabled(false);
    }
}

// Function to calibrate moisture sensor readings
QList<int> MainWindow::calibrateMoistureValues(const QList<int>& data_h, const QList<int>& data_L, const QList<int>& vals)
{
    // List to store calibrated values
    QList<int> calibratedValues;
    // Iterate over each sensor
    for (int i = 0; i < 800; i++) {
        int val = vals[i];

        // Find the nearest values in the calibration matrices for the current sensor
        int nearest_h = data_h[i];
        int nearest_L = data_L[i];

        // Perform calibration for the current sensor
        double calibratedValue = static_cast<double>(val - nearest_h) / (nearest_L - nearest_h);
        calibratedValues.append(calibratedValue);

    }


    return calibratedValues;
}

//-----------------------------------------------------------//
void MainWindow::smoothColumnsAroundLowest(QVector<QVector<int>>& matrix)
{
    int numRows = matrix.size();
    int numCols = matrix[0].size();

    for (int col = 0; col < numCols; col++)
    {
        // Find the lowest value in the column
        int lowestValue = matrix[0][col];
        for (int row = 1; row < numRows; row++)
        {
            if (matrix[row][col] < lowestValue)
                lowestValue = matrix[row][col];
        }

        // Smooth the values around the lowest value
        for (int row = 0; row < numRows; row++)
        {
            int currentValue = matrix[row][col];

            // Smooth the value if it is not the lowest value
            if (currentValue != lowestValue)
            {
                int smoothedValue = (currentValue + lowestValue) / 2;
                if(smoothedValue > currentValue){
                    matrix[row][col] = smoothedValue;
                }
                else{
                    int randomOffset = qrand() % 5 - 2; // Random offset between -2 and 2
                    int smoothedVal = lowestValue + randomOffset;
                    if(smoothedVal >= 100)
                        matrix[row][col] = 100;
                    else if(smoothedVal <= 0)
                        matrix[row][col] = 0;
                    else
                        matrix[row][col] = smoothedVal;
//                    matrix[row][col] = lowestValue;
                }
            }
        }
    }
    for (int row = 0; row < numRows; row++)
    {
        for (int col = 1; col < numCols; col++)
        {
//            qDebug() <<matrix[row][col];
        }
//        qDebug() << "--";
    }
}

void MainWindow::clearCheck(QList<int>& matrix)
{
    int highValsCount = 0;
    int lowValsCount = 0;
    for (int i = 0; i < matrix.size(); ++i) {
        if(matrix.at(i) >= 60){
            highValsCount++;
        }
        if(matrix.at(i) <= 20){
            lowValsCount++;
        }
    }
    qDebug() << highValsCount;
    if (highValsCount > 700) {
        for (int i = 0; i < matrix.size(); ++i) {
            int randomOffset = qrand() % 5 - 5; // Random offset between -2 and 2
            int smoothedValue = 95 + randomOffset;
            matrix[i] = smoothedValue;
        }
    }
    qDebug() << lowValsCount;
    if (lowValsCount > 700) {
        for (int i = 0; i < matrix.size(); ++i) {
            int randomOffset = qrand() % 5 - 5; // Random offset between -2 and 2
            int smoothedValue = 5 + randomOffset;
            matrix[i] = smoothedValue;
        }
    }
}

void MainWindow::blueCheck(QList<int>& matrix)
{
    int mat1[ROWS][COLS];
    int mat2[ROWS][COLS];
    int zeroCount1[16];
    int zeroCount2[16];
    int cnt1 = 0;
    int cnt2 = 400;

    for (int i = 0; i < 16; ++i) {
        zeroCount1[i] = 0;
        zeroCount2[i] = 0;
    }

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mat1[i][j] = matrix.at(cnt1++);
        }
    }
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            mat2[i][j] = matrix.at(cnt2++);
        }
    }

    // Count zeros in each column
    for (int col = 0; col < COLS; col++)
    {
        for (int row = 0; row < ROWS; row++)
        {
            if (mat1[row][col] == 0.0)
                zeroCount1[col]++;
            if (mat2[row][col] == 0.0)
                zeroCount2[col]++;
        }
    }

    // Display the results
    for (int col = 0; col < COLS; col++)
    {
        if(zeroCount1[col] <= 5 ){
            for (int col = 0; col < COLS; col++)
            {
                for (int row = 0; row < ROWS; row++)
                {
                    if (mat1[row][col] == 0.0)
                        mat1[row][col] = 5;
                }
            }
        }
        if(zeroCount2[col] <= 5 ){
            for (int col = 0; col < COLS; col++)
            {
                for (int row = 0; row < ROWS; row++)
                {
                    if (mat2[row][col] == 0.0)
                        mat2[row][col] = 5;
                }
            }
        }

        if(zeroCount1[col] <= 10 ){
            for (int col = 0; col < COLS; col++)
            {
                for (int row = 0; row < ROWS; row++)
                {
                    if (mat1[row][col] == 0.0)
                        mat1[row][col] = 1;
                }
            }
        }
        if(zeroCount2[col] <= 10 ){
            for (int col = 0; col < COLS; col++)
            {
                for (int row = 0; row < ROWS; row++)
                {
                    if (mat2[row][col] == 0.0)
                        mat2[row][col] = 1;
                }
            }
        }

        cnt1 = 0;
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                matrix[cnt1++] = mat1[i][j];
            }
        }
        cnt2 = 400;
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                matrix[cnt2++] = mat2[i][j];
            }
        }

        //        qDebug() << "Column1" << col << "has" << zeroCount1[col] << "zeros";
        //        qDebug() << "Column2" << col << "has" << zeroCount2[col] << "zeros";
    }
}



