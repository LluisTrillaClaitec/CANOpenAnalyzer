#ifndef CANFRAMEMODEL_H
#define CANFRAMEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QDebug>
#include <QMutex>
#include "can_structs.h"
#include "dbc/dbchandler.h"
#include "connections/canconnection.h"

enum class Column {
    TimeStamp       = 0,  ///< The timestamp when the frame was transmitted or received
    FrameId         = 1,  ///< The frames CAN identifier (Standard: 11 or Extended: 29 bit)
    CANOpenNode     = 2, ///< CANOpen Node
    CANOpenFunction = 3,  ///< CANOpen Function Code
    Data            = 4,  ///< The frames payload data
    Remote          = 5,  ///< True if the frames is a remote frame
    Direction       = 6,  ///< Whether the frame was transmitted or received
    Bus             = 7,  ///< The bus where the frame was transmitted or received
    Extended        = 8,  ///< True if the frames CAN identifier is 29 bit
    Length          = 9,  ///< The frames payload data length
    ASCII           = 10,  ///< The payload interpreted as ASCII characters
    NUM_COLUMN
};

class CANFrameModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    CANFrameModel(QObject *parent = 0);
    virtual ~CANFrameModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &) const;
    int totalFrameCount();

    void sendRefresh();
    void sendRefresh(int);
    int  sendBulkRefresh();
    void clearFrames();
    void setInterpretMode(bool);
    bool getInterpretMode();
    void setOverwriteMode(bool);
    void setHexMode(bool);
    void setSysTimeMode(bool);
    void setFilterState(unsigned int ID, bool state);
    void setAllFilters(bool state);
    void setSecondsMode(bool);
    void setTimeFormat(QString);
    void loadFilterFile(QString filename);
    void saveFilterFile(QString filename);
    void normalizeTiming();
    void recalcOverwrite();
    bool needsFilterRefresh();
    void insertFrames(const QVector<CANFrame> &newFrames);
    void sortByColumn(int column);
    int getIndexFromTimeID(unsigned int ID, double timestamp);
    const QVector<CANFrame> *getListReference() const; //thou shalt not modify these frames externally!
    const QVector<CANFrame> *getFilteredListReference() const; //Thus saith the Lord, NO.
    const QMap<int, bool> *getFiltersReference() const; //this neither
    void set_filterNMTon(bool state) {filterNMTon = state; sendRefresh();};
    void set_filterSYNCon(bool state) {filterSYNCon = state; sendRefresh();};
    void set_filterEMCYon(bool state) {filterEMCYon = state; sendRefresh();};
    void set_filterHBEATon(bool state) {filterHBEATon = state; sendRefresh();};
    void set_filterTIMEon(bool state) {filterTIMEon = state; sendRefresh();};
    void set_filterTPDOon(bool state) {filterTPDOon = state; sendRefresh();};
    void set_filterLSSon(bool state) {filterLSSon = state; sendRefresh();};
    bool filterFrameConsideringFunction(int frame_id);

    QString printSDO(int sdo, const unsigned char *data) const;

public slots:
    void addFrame(const CANFrame&, bool);
    void addFrames(const CANConnection*, const QVector<CANFrame>&);

signals:
    void updatedFiltersList();

private:
    void qSortCANFrameAsc(QVector<CANFrame>* frames, Column column, int lowerBound, int upperBound);
    void qSortCANFrameDesc(QVector<CANFrame>* frames, Column column, int lowerBound, int upperBound);
    uint64_t getCANFrameVal(int row, Column col);
    bool any_filters_are_configured(void);

    QVector<CANFrame> frames;
    QVector<CANFrame> filteredFrames;
    QMap<int, bool> filters;
    bool filterNMTon = false;
    bool filterSYNCon = false;
    bool filterEMCYon = false;
    bool filterHBEATon = false;
    bool filterTIMEon = false;
    bool filterTPDOon = false;
    bool filterLSSon = false;
    DBCHandler *dbcHandler;
    QMutex mutex;
    bool interpretFrames; //should we use the dbcHandler?
    bool overwriteDups; //should we display all frames or only the newest for each ID?
    QString timeFormat;
    bool useHexMode;
    bool timeSeconds;
    bool useSystemTime;
    bool needFilterRefresh;
    int64_t timeOffset;
    int lastUpdateNumFrames;
    uint32_t preallocSize;
    bool sortDirAsc;
    QString printIndexSubIndex(const unsigned char *data) const;
};


#endif // CANFRAMEMODEL_H

