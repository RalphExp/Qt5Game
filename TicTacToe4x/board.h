#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <mutex>
#include <future>

#include <QObject>

using namespace std;

class MainWindow;

class Board : public QObject {
Q_OBJECT
public:
    static const int N = 0;
    static const int O = 1;
    static const int X = 2;
    static const int kNotStart = 0;
    static const int kPlaying = 1;
    static const int kEnded = 2;
    static const int kFirstPlay = 0;
    static const int kSecondPlay = 1;

    Board();
    virtual ~Board();

    int getState() { return stat_; }
    int getPlayer() { return player_; }

    void setWindow(MainWindow* win);

    bool isFirstPlay(void) { return first_play_; }

    /* @brief: player or computer? */
    bool isPlayerTurn(void);

    /* @brief: if valid return 1, else return 0 */
    int setBoard(int b, int x, int y);

    /* @brief: start game */
    void start(bool firstPlay);

    /* @brief: reset board, clear all the data. */
    void reset(void);

    /* @brief: return 1 if gameover 0 otherwise. */
    int next(int b, int x, int y);

signals:
    void computerDone(void);

public:
    int board_[4][4][4];
    bool checkGameOver(int b, int x, int y);

private:
    int stat_;
    int player_;
    bool first_play_;

    MainWindow* win_;
    std::mutex mtx_;
    std::future<void> future_;
};

#endif // BOARD_H
