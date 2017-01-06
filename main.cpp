#include <QCoreApplication>
#include <QFile>
#include <QTimer>
#include <QUdpSocket>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

int main(int argc,char* argv[]){

    QCoreApplication app(argc,argv);

    QFile config("config.json");
    config.open(QIODevice::ReadOnly);

    QByteArray configs = config.readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(configs,&err);
    if (err.error==QJsonParseError::NoError){

        QList<QString> ips;
        uint port = 6666;
        uint period = 5000;
        QList<QByteArray> message;

        if (doc.object().contains("address")){
            auto addrV = doc.object().value("address").toArray().toVariantList();
            foreach (auto i, addrV){
                QString s = i.toString();
                if (!(s.isNull() || s.isEmpty())){
                    ips << s;
                }
            }
        }

        if (doc.object().contains("port")){
            uint tp = doc.object().value("port").toInt();
            if (tp>0 && tp<65000){
                port = tp;
            }
        }

        if (doc.object().contains("period")){
            uint tp = doc.object().value("period").toInt();
            if (tp>100 && tp<600000){
                period = tp;
            }
        }
        if (doc.object().contains("message")){
            QJsonArray msg = doc.object().value("message").toArray();
            if (!msg.isEmpty()){
                for (int i=0; i<msg.size();i++){
                    message.append(msg.at(i).toString().toLatin1());
                }
            }
        }

        QTimer tmr;
        QUdpSocket socket;
        QObject::connect(&tmr,&QTimer::timeout,[&](){
            foreach (auto dst, ips) {
                foreach (QByteArray m, message){
                    socket.writeDatagram(m,QHostAddress(dst),port);
                    qDebug() << "send";
                }
            }
        });

        tmr.start(period);

        app.exec();
    } else {
        qWarning() << QString("Failed to parse json %0").arg(err.errorString());
    }

    return 0;
}
