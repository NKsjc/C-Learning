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
            SolidBrush sb = new SolidBrush(blockColor );
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
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (strArr[i,j]==true)
                    {
                        gp.FillRectangle(sb, 31 * i + 1, 31 * j + 1, 30, 30);
                    }
                }
                
            }
            //在这不能释放gp......
            //gp.Dispose();//释放gp
        }
        /// <summary>
        /// 2015.03.13
        /// 方格点击事件，判断点击方块的反应事件
        /// strArr是bool型数组，记录方格的值来对应方格的颜色
        /// 初始化时值都为false
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void lblModel_MouseClick(object sender, MouseEventArgs e)
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
            Graphics gp = lblModel.CreateGraphics();
            SolidBrush sb = new SolidBrush(change?blockColor:Color.Black);
            gp.FillRectangle(sb,31*xPos+1,31*yPos+1,30,30);
            gp.Dispose();//释放gp
        }

        private void lblColor_Click(object sender, EventArgs e)
        {
            colorDialog1.ShowDialog();
            blockColor = colorDialog1.Color;
            lblColor.BackColor = blockColor;
            lblModel.Invalidate();
        }
        /// <summary>
        /// 2015.03.13
        /// 添加设置的砖块格式，显示在lsvBlock中
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnAdd_Click(object sender, EventArgs e)
        {
            bool isEmpty = false;
            foreach (var item in strArr)
            {
                if (item==true)
                {
                    isEmpty = true;
                    break;

                }
            }
            if (isEmpty==false)
            {
                MessageBox.Show("图像为空，请绘制图像","提示窗口");
                return;
            }
            StringBuilder strBuilder = new StringBuilder(25);
            foreach (var item in strArr)
            {
                strBuilder.Append(item?"1":"0");
            }
            string str = strBuilder.ToString();
            foreach (ListViewItem item in lsvBlockSet.Items)
            {
                if (item.SubItems[0].Text==str)
                {
                    MessageBox.Show("图案已经存在", "提示窗口");
                    return;
                }
            }
            ListViewItem lvi = new ListViewItem();
            lvi = lsvBlockSet.Items.Add(str);
            lvi.SubItems.Add(blockColor.ToArgb().ToString());
        }
        /// <summary>
        /// 2015.03.13
        /// 选择lsvBlockSet中的不同样式，显示出来
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void lsvBlockSet_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (e.IsSelected)
            {
                blockColor = Color.FromArgb(int.Parse(e.Item.SubItems[1].Text));
                string str = e.Item.SubItems[0].Text.ToString();
                for (int i = 0; i < 25; i++)
                {
                    strArr[i / 5, i % 5] =(str[i]=='1')?true:false;
                }
                lblModel.Invalidate();
            }
        }
        /// <summary>
        /// 2015.03.13
        /// 删除列表中选中的样式，并且在图案框中清空
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDel_Click(object sender, EventArgs e)
        {
            if (lsvBlockSet.SelectedItems.Count==0)
            {
                MessageBox.Show("请选择一个项目");
                return;
            }
            lsvBlockSet.Items.Remove(lsvBlockSet.SelectedItems[0]);
            btnClear.PerformClick();
        }
        /// <summary>
        /// 清空图案框中的内容
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    strArr[i,j] = false;
                }
            }
            lblModel.Invalidate();
        }
        /// <summary>
        /// 修改列表中的图案样式
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUpdate_Click(object sender, EventArgs e)
        {
            bool isEmpty = false;
            foreach (var item in strArr)
            {
                if (item == true)
                {
                    isEmpty = true;
                    break;
                }
            }
            if (isEmpty == false)
            {
                MessageBox.Show("图像为空，请绘制图像", "提示窗口");
                return;
            }
            if (lsvBlockSet.SelectedItems.Count == 0)
            {
                MessageBox.Show("请选择一个项目");
                return;
            }
            StringBuilder strBuilder = new StringBuilder(25);
            foreach (var item in strArr)
            {
                strBuilder.Append(item?"1":"0");
            }
            lsvBlockSet.SelectedItems[0].SubItems[0].Text = strBuilder.ToString();
            lsvBlockSet.SelectedItems[0].SubItems[1].Text = Convert.ToString(blockColor.ToArgb());
        }

      
    }
}
