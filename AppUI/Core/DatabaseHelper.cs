using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Data.Odbc;
using System.Data.OleDb;

namespace AppUI.Core
{
    public class DatabaseHelper : DispatcherObject
    {
        public void test()
        {
            OleDbConnectionStringBuilder oleString = new OleDbConnectionStringBuilder();
            //为了使大家更清楚使用这个类，制造一个连接字符串
            oleString.Provider = "Microsoft.ACE.OleDB.12.0";
            //使用刚刚安装的数据库引擎，大家不要写错了
            oleString.DataSource = @".\QRCodeData.accdb";
            //这里写你数据库连接的位置
            OleDbConnection conn = new OleDbConnection();
            //创建OleDb连接对象
            conn.ConnectionString = oleString.ToString();
            //将生成的字符串传入
            conn.Open();
            //打开数据库
            OleDbCommand mycmd = new OleDbCommand();
            //创建sql命令对象
            mycmd.Connection = conn;
            //设置连接
            mycmd.CommandText = "Insert into LensQRCode(ID,DateT,Type,message,Result) values(@ID,@DateT,@Type,@message,@Result)";
            //并且用sql参数形式插入数据
            mycmd.Parameters.AddWithValue("@ID", "6666");
            mycmd.Parameters.AddWithValue("@DateT", "6666");
            mycmd.Parameters.AddWithValue("@Type", "6666");
            mycmd.Parameters.AddWithValue("@message", "6666");
            mycmd.Parameters.AddWithValue("@Result", "6666");

            //加入参数值
            mycmd.ExecuteNonQuery();
            //执行插入语句

            string sql = "select * from LensQRCode ";
            OleDbCommand aCommand = new OleDbCommand(sql, conn);
            OleDbDataReader reader = aCommand.ExecuteReader();
            while (reader.Read())

            {

                var t1 = reader["ID"].ToString();

                Console.WriteLine(t1.ToString());

            }

            conn.Close();
            //最后不要忘了关数据库
            mycmd.Dispose();
        }

        //OleDb连接对象
        public OleDbConnection DataBaseConn = new OleDbConnection();

        //初始化，创立数据库连接
        public void init(string path)
        {
            //创建一个连接字符串
            OleDbConnectionStringBuilder oleString = new OleDbConnectionStringBuilder();

        }
    }
}
