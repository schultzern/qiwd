#include "Window.h"

#include <QDBusConnection>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>

Window::Window(QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);

    QLabel *interfaceLabel = new QLabel(this);
    interfaceLabel->setText("Interface");
    m_deviceList = new QComboBox;
    layout()->addWidget(interfaceLabel);
    layout()->addWidget(m_deviceList);

    QLabel *networksLabel = new QLabel(this);
    networksLabel->setText("Available Networks");
    m_networkList = new QListWidget;
    layout()->addWidget(networksLabel);
    layout()->addWidget(m_networkList);

    QLabel *knownNetworkLabel = new QLabel(this);
    knownNetworkLabel->setText("Known Networks");
    m_knownNetworksList = new QListWidget;
    layout()->addWidget(knownNetworkLabel);
    layout()->addWidget(m_knownNetworksList);

    connect(&m_iwd, &Iwd::visibleNetworkAdded, this, [=](const QString &name){
        m_networkList->addItem(name);
    });
    connect(&m_iwd, &Iwd::visibleNetworkRemoved, this, [=](const QString &name){
        for (const QListWidgetItem *item : m_networkList->findItems(name, Qt::MatchExactly)) {
            delete m_networkList->takeItem(m_networkList->row(item));
        }
    });

    connect(&m_iwd, &Iwd::knownNetworkAdded, this, [=](const QString &name, const QString &id){
        Q_UNUSED(id);
        m_knownNetworksList->addItem(name);
    });
    connect(&m_iwd, &Iwd::knownNetworkRemoved, this, [=](const QString &name){
        for (const QListWidgetItem *item : m_knownNetworksList->findItems(name, Qt::MatchExactly)) {
            delete m_knownNetworksList->takeItem(m_knownNetworksList->row(item));
        }
    });

    connect(&m_iwd, &Iwd::deviceAdded, this, [=](const QString &name){
        m_deviceList->addItem(name);
    });
    connect(&m_iwd, &Iwd::deviceRemoved, this, [=](const QString &name){
        for (int i=0; i<m_deviceList->count(); i++) {
            if (m_deviceList->itemText(i) == name) {
                m_deviceList->removeItem(i);
                return;
            }
        }
    });

    m_iwd.init();
}
