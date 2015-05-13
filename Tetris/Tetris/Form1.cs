using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Tetris
{
    public partial class frmConfigure : Form
    {
        public frmConfigure()
        {
            InitializeComponent();
        }

        private bool[,] strArr = new bool[5, 5];
        private Color blockColor = Color.Red;

        /// <summary>
        /// 2015.05.13，完成砖块的模型
        /// 画出一个5*5的方格
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void labelModel_Paint(object sender, PaintEventArgs e)
        {
            Graphics gp = e.Graphics;
            gp.Clear(Color.Black);
            Pen pDraw = new Pen(Color.White);
            Point ps, pe;
            for (int i = 31; i < 155; i=i+31)
            {
                ps=new Point(1,i);
                pe=new Point(155,i);
                gp.DrawLine(pDraw,ps,pe);
            }
            for (int i = 31; i < 155; i = i + 31)
            {
                ps = new Point(i, 1);
                pe = new Point(i, 155);
                gp.DrawLine(pDraw, ps, pe);
            }
            gp.Dispose();//释放gp
        }
        /// <summary>
        /// 2015.03.13
        /// 方格点击事件，判断点击方块的反应事件
        /// strArr是bool型数组，记录方格的值来对应方格的颜色
        /// 初始化时值都为false
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void labelModel_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button!=MouseButtons.Left)//判断是左键or右键
            {
                return;
            }
            int xPos, yPos;
            xPos = e.X / 31;
            yPos = e.Y / 31;
            strArr[xPos, yPos] = !strArr[xPos, yPos];
            bool change = strArr[xPos, yPos];
            Graphics gp = labelModel.CreateGraphics();
            SolidBrush sb = new SolidBrush(change?blockColor:Color.Black);
            gp.FillRectangle(sb,31*xPos+1,31*yPos+1,30,30);
            gp.Dispose();//释放gp
        }
    }
}
