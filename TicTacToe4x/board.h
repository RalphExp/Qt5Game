#ifndef BOARD_H
#define BOARD_H

#include <mutex>
#include <future>
#include <QObject>

using namespace std;

class MainWindow;

class Grid {
public:
    Grid(int b = 0, int x = 0, int y = 0) : b(b),x(x),y(y){}
    bool operator==(const Grid& g) {
        return b == g.b && x == g.x && y == g.y;
    }
    int b = 0;
    int x = 0;
    int y = 0;
};

typedef vector<Grid> Path;

class Board : public QObject {
Q_OBJECT
public:
    /* grid stat */
    static const int NN = 0;
    static const int OO = 1;
    static const int XX = 2;

    /* game stat */
    static const int kNotStart = 0;
    static const int kPlaying = 1;
    static const int kFirstWin = 2;
    static const int kSecondWin = 3;
    static const int kDraw = 4;

    /* player stat */
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
    int getBoard(int b, int x, int y);

    /* @brief: if valid return 1, else return 0 */
    int setBoard(int b, int x, int y);

    /* @brief: start game */
    void start(bool firstPlay);

    /* @brief: reset board, clear all the data. */
    void reset(void);

    /* @brief: computer's turn */
    void next();

    /* index by grid */
    int operator[](const Grid& g);

    Path& getWinPath() { return win_path_; }

signals:
    void computerDone(int);

public:
    int board_[4][4][4];
    void checkGameOver(int b, int x, int y);

private:
    /* @brief: get all the paths connected to the given point. */
    vector<Path> getPaths(int b, int x, int y);
    int getScore(int b, int x, int y);

    int steps_;
    int stat_;
    int player_;
    bool first_play_;

    MainWindow* win_;
    std::mutex mtx_;
    std::future<void> future_;
    Path win_path_;
};

#endif // BOARD_H
