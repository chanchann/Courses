package lora.util;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class DBUtil
{
    static String ip="192.168.154.128";
    static int port=3306;
    static String database="lora";
    static String encoding="UTF-8";
    static String loginName="root";
    static String password="12345678";

    static
    {
        try
        {
            Class.forName("com.mysql.jdbc.Driver");
        }catch(ClassNotFoundException e)
        {
            e.printStackTrace();
        }
    }

    public static Connection getConnection() throws SQLException
    {
        String url=String.format("jdbc:mysql://%s:%d/%s?characterEncoding=%s",ip,port,database,encoding);
        return DriverManager.getConnection(url,loginName,password);
    }
    public static void main(String[] args) throws SQLException
    {
        System.out.println(getConnection());
    }

}
