
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.util.Random;
import java.util.Stack;

import javax.swing.JComponent;
import javax.swing.JFrame;

public class Maze {
  private final int X_SIZE = 50;
  private final int Y_SIZE = 50;

  // all cells
  private boolean visited[][] = null;

  // four walls of one cell
  private boolean north[][] = null;
  private boolean south[][] = null;
  private boolean east[][] = null;
  private boolean west[][] = null;

  // to find a way out of the maze
  private boolean explored[][] = null;

  private JComponent myCanvas = null;

  private Cell current_ = null;

  class Cell {
    private final int x;
    private final int y;
    public Cell(int x, int y) {
      this.x = x;
      this.y = y;
    }

    public int getX() {
      return x;
    }
    public int getY() {
      return y;
    }
  }

  public int getXBound() {
    return X_SIZE;
  }

  public int getYBound() {
    return Y_SIZE;
  }

  public Maze() {
    // extend SIZE by 2 as the outside cells
    // whose state is always set as visited=yes,
    // so that you can never cross the boundary cells
    // Note +2 is necessary to avoid repeated bound checking code
    visited = new boolean[X_SIZE + 2][Y_SIZE + 2];
    for (int x = 1; x < X_SIZE + 1; x++) {
      for (int y = 1; y < Y_SIZE + 1; y++) {
        visited[x][y] = false;
      }
    }

    // initial the boundary cells as already visited
    for (int y = 0; y < Y_SIZE + 2; y++) {
      visited[0][y] = true;
      visited[X_SIZE + 1][y] = true;
    }

    for (int x = 0; x < X_SIZE + 2; x++) {
      visited[x][0] = true;
      visited[x][Y_SIZE + 1] = true;
    }

    // initialize the four-direction walls
    north = new boolean[X_SIZE + 2][Y_SIZE + 2];
    south = new boolean[X_SIZE + 2][Y_SIZE + 2];
    east = new boolean[X_SIZE + 2][Y_SIZE + 2];
    west = new boolean[X_SIZE + 2][Y_SIZE + 2];

    for (int x = 0; x < X_SIZE + 2; x++) {
      for (int y = 0; y < Y_SIZE + 2; y++) {
        north[x][y] = true;
        south[x][y] = true;
        east[x][y] = true;
        west[x][y] = true;
      }
    }
  }

  public void findPath(int startX, int startY, int endX, int endY) {
    explored = new boolean[X_SIZE + 2][Y_SIZE + 2];
    for (int x = 0; x < X_SIZE + 2; x++) {
      for (int y = 0; y < Y_SIZE + 2; y++) {
        explored[x][y] = false;
      }
    }

    // set the boundary cells as already explored
    for (int y = 0; y < Y_SIZE + 2; y++) {
      explored[0][y] = true;
      explored[X_SIZE + 1][y] = true;
    }

    for (int x = 0; x < X_SIZE + 2; x++) {
      explored[x][0] = true;
      explored[x][Y_SIZE + 1] = true;
    }

    explorePath(1, 1, explored);
  }

  private void explorePath(int x, int y, boolean explored[][]) {
    explored[x][y] = true;
    if (x == X_SIZE + 1 && y == Y_SIZE + 1) {
      System.out.println("find a path!");
      return;
    }

    if (!north[x][y] && !explored[x][y + 1]) {
      explorePath(x, y + 1, explored);
    } else if (!south[x][y] && !explored[x][y - 1]) {
      explorePath(x, y - 1, explored);
    } else if (!east[x][y] && !explored[x + 1][y]) {
      explorePath(x + 1, y, explored);
    } else if (!west[x][y] && !explored[x - 1][y]) {
      explorePath(x - 1, y, explored);
    } else {
      System.out.println("dead end");
      return;
    }
  }

  private final int zoomPixel = 10;

  private void pickNextCell(Cell current, Stack<Cell> cellStack) {
    if (cellStack.isEmpty()) {
      return;
    }

    visited[current.getX()][current.getY()] = true;
    System.out.println("visit: " + current.getX() + ", " + current.getY());

    Cell next = null;
    Random random = new Random();

    if (!visited[current.getX()][current.getY() + 1] || !visited[current.getX() + 1][current.getY()]
        || !visited[current.getX()][current.getY() - 1]
        || !visited[current.getX() - 1][current.getY()]) {
      while (true) {
        int ran = random.nextInt(100);

        if (ran < 25 && !visited[current.getX() + 1][current.getY()]) {
          next = new Cell(current.getX() + 1, current.getY());
          cellStack.push(next);
          east[current.getX()][current.getY()] = false;
          west[current.getX() + 1][current.getY()] = false;
          break;

        } else if (ran >= 25 && ran < 50 && !visited[current.getX() - 1][current.getY()]) {
          next = new Cell(current.getX() - 1, current.getY());
          cellStack.push(next);
          west[current.getX()][current.getY()] = false;
          east[current.getX() - 1][current.getY()] = false;
          break;
        } else if (ran >= 50 && ran < 75 && !visited[current.getX()][current.getY() + 1]) {
          next = new Cell(current.getX(), current.getY() + 1);
          cellStack.push(next);
          north[current.getX()][current.getY()] = false;
          south[current.getX()][current.getY() + 1] = false;
          break;
        } else if (ran >= 75 && ran < 100 && !visited[current.getX()][current.getY() - 1]) {
          next = new Cell(current.getX(), current.getY() - 1);
          cellStack.push(next);
          south[current.getX()][current.getY()] = false;
          north[current.getX()][current.getY() - 1] = false;
          break;
        }
      }

    } else {
      // if the current cell has no un-visited neighbours, then it's dead-end,
      // we backtrace to pick next cell from our stack
      next = (Cell) cellStack.pop();
    }

    current_ = current;
    myCanvas.repaint();

    try {
      Thread.sleep(50);
    } catch (Exception e) {
    }

    // graphics.setColor(new Color(200,200,200,150)); // set back to the background color
    // graphics.fillRect(current.getX()*zoomPixel , current.getY()*zoomPixel, zoomPixel, zoomPixel);

    pickNextCell(next, cellStack);
  }

  public void createMaze() {
    Stack<Cell> cellStack = new Stack<Cell>();

    int x = 1, y = 1; // start point
    Cell exit = new Cell(x, y);
    cellStack.push(exit);
    pickNextCell(exit, cellStack);
  }

  public void initUI() {
    JFrame frame = new JFrame();
    frame.setTitle("WJ MAZE");
    // frame.setSize(300, 200);
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

    myCanvas = new JComponent() {
      final int pixel = 10;

      protected void paintComponent(Graphics g) {
        final Color backColor = new Color(200, 200, 200, 150);
        final Color wallColor = new Color(0, 0, 0, 150);

        int pixel = 10;
        System.out.println("we are painint ...");
        // paint the background ignoring the boundary cells
        g.setColor(backColor);
        g.fillRect(1 * zoomPixel, 1 * zoomPixel, X_SIZE * zoomPixel, Y_SIZE * zoomPixel);

        /*
                        // paint the cells, but since all cells will be visited
                        // when creating the maze, there's no need for this code
                        for (int x = 0; x < X_SIZE+2; x++){
                            for (int y = 0; y < Y_SIZE+2; y++){
                                if (!visited[x][y]){
                                    g.fillRect(x * zoomPixel , y * zoomPixel, zoomPixel, zoomPixel);
                                }
                            }
                        }
        */
        g.setColor(wallColor);

        // draw the maze walls ignoring the boundary cells and walls
        for (int x = 1; x < X_SIZE + 1; x++) {
          for (int y = 1; y < Y_SIZE + 1; y++) {
            if (north[x][y]) {
              g.drawLine(
                  x * zoomPixel, (y + 1) * zoomPixel, (x + 1) * zoomPixel, (y + 1) * zoomPixel);
            }

            if (south[x][y]) {
              g.drawLine(x * zoomPixel, y * zoomPixel, (x + 1) * zoomPixel, y * zoomPixel);
            }

            if (east[x][y]) {
              g.drawLine(
                  (x + 1) * zoomPixel, y * zoomPixel, (x + 1) * zoomPixel, (y + 1) * zoomPixel);
            }

            if (west[x][y]) {
              g.drawLine(x * zoomPixel, y * zoomPixel, x * zoomPixel, (y + 1) * zoomPixel);
            }
          }
        }

        if (current_ != null) {
          g.setColor(Color.RED);
          g.fillRect(current_.getX() * zoomPixel + 2, current_.getY() * zoomPixel + 2,
              zoomPixel - 4, zoomPixel - 4);
        }
      }

      public Dimension getPreferredSize() {
        return new Dimension(
            (X_SIZE + 2) * pixel + zoomPixel * 2, (Y_SIZE + 2) * pixel + zoomPixel * 2);
      }

      public Dimension getMinimumSize() {
        return getPreferredSize();
      }
    };

    frame.getContentPane().add(myCanvas, BorderLayout.CENTER);
    frame.pack();
    frame.setVisible(true);
  }

  public static void main(String[] args) {
    Maze maze = new Maze();
    maze.initUI();
    maze.createMaze();
  }
}
