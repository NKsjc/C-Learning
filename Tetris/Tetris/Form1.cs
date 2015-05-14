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
        Configure congfig = new Configure();
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
            //使用var在这里是不可以的
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
            //生成按钮的点击事件
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
            /*2015.03.14修复bug
             * 由于lsvBlockSet_ItemSelectionChanged是根据选择的行改变事件
             * 因此，清空图案后，再点击相同的行，不会引发该事件，
             * 需要清空已经选择的行，从而引发事件
             * */
            lsvBlockSet.SelectedItems.Clear();
            lblModel.Invalidate();//重绘控件
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
        /// <summary>
        /// 2015.03.14
        /// 判断键入的快捷键
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtContra_KeyDown(object sender, KeyEventArgs e)
        {
            if ((e.KeyValue>=33)&&(e.KeyValue<=36)||(e.KeyValue>=45)&&(e.KeyValue<=46)||
                (e.KeyValue >= 48) && (e.KeyValue <= 57) || (e.KeyValue >= 65) && (e.KeyValue <= 90)||
                (e.KeyValue >= 96) && (e.KeyValue <= 107) || (e.KeyValue >= 109) && (e.KeyValue <= 111)||
                (e.KeyValue >= 186) && (e.KeyValue <= 192) || (e.KeyValue >= 219) && (e.KeyValue <= 222))
            {
                foreach (Control c in gbKeySet.Controls)
                {
                    Control temp = c as TextBox;
                    if (temp != null && ((TextBox)temp).Text!="")
                    {
                        if (((int)((TextBox)temp).Tag)==e.KeyValue)
                        {
                            ((TextBox)temp).Text = "";
                            ((TextBox)temp).Tag = Keys.None;
                        }
                    }
                }
                //((TextBox)sender).Text = e.KeyCode.ToString();
                ((TextBox)sender).Tag = (Keys)e.KeyValue;
            }
        }

        private void lblBackgroundColor_Click(object sender, EventArgs e)
        {
            //if (e.vMouseButtons.Left )
            //{
                
            //}
            colorDialog1.ShowDialog();
            lblBackgroundColor.BackColor = colorDialog1.Color;

        }

        private void frmConfigure_Load(object sender, EventArgs e)
        {
            congfig.LoadFromXmlFile();
            ArrInfo info = new ArrInfo();
            info = congfig.Info;
            ListViewItem lsv = new ListViewItem();
            for (int i = 0; i < info.Legth; i++)
            {
                lsv = lsvBlockSet.Items.Add(info[i].GetIdStr());
                lsv.SubItems.Add(info[i].GetColorStr());
            }
            txtLeft.Text =((Keys) congfig.MoveLeftKey).ToString();
            txtLeft.Tag = congfig.MoveLeftKey;
            txtRight.Text = ((Keys)congfig.MoveRightKey).ToString();
            txtRight.Tag = congfig.MoveRightKey;
            txtDown.Text = ((Keys)congfig.DownKey).ToString();
            txtDown.Tag = congfig.DownKey;
            txtDrop.Text = ((Keys)congfig.DropKey).ToString();
            txtDrop.Tag = congfig.DropKey;
            txtDeasil.Text = ((Keys)congfig.DeasilRotateKey).ToString();
            txtDeasil.Tag = congfig.DeasilRotateKey;
            txtContra.Text = ((Keys)congfig.ContraRotateKey).ToString();
            txtContra.Tag = congfig.ContraRotateKey;
            txtCoorWidth.Text = (congfig.CoorWidth).ToString();
            txtCoorWidth.Tag = congfig.CoorWidth;
            txtCoorHigh.Text = (congfig.CoorHigh).ToString();
            txtCoorHigh.Tag = congfig.CoorHigh;
            txtCoorColor.Text = (congfig.RectPix).ToString();
            txtCoorColor.Tag = congfig.RectPix;
            lblBackgroundColor.BackColor = congfig.BackColor;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            ArrInfo info = new ArrInfo();
            foreach (ListViewItem item in lsvBlockSet.Items)
            {
                info.Add(item.SubItems[0].Text,item.SubItems[1].Text);
            }
            congfig.Info = info;
            //需要添加其它的信息
            congfig.SaveToXmlFile();
        }



      
    }
}
