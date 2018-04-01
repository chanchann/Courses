package lora.lora.dao;

import lora.bean.Address;
import lora.util.DBUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class AddressDAO
{
    public int getTotal()
    {
        int total=0;
        try(Connection c= DBUtil.getConnection(); Statement s=c.createStatement();)
        {
            String sql="select count(*) from address";

            ResultSet rs=s.executeQuery(sql);
            while(rs.next())
            {
                total=rs.getInt(1);
            }
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        return total;
    }

    public void add(Address bean)
    {
        String sql="insert into address values(null,?)";
        try(Connection c=DBUtil.getConnection();PreparedStatement ps=c.prepareStatement(sql);)
        {
            ps.setString(1,bean.getName());
            ps.execute();
            ResultSet rs=ps.getGeneratedKeys();
            if(rs.next())
            {
                int id=rs.getInt(1);
                bean.setId(id);
            }
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
    }

    public void update(Address bean)
    {
        String sql="update address set name=? where id=?";
        try(Connection c=DBUtil.getConnection();PreparedStatement ps=c.prepareStatement(sql))
        {
            ps.setString(1,bean.getName());
            ps.setInt(2,bean.getId());
            ps.execute();
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
    }

    public void delete(int id)
    {
        try(Connection c=DBUtil.getConnection();Statement s=c.createStatement();)
        {
            String sql="delete from address where id="+id;
            s.execute(sql);
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
    }

    public Address get(int id)
    {
        Address bean=null;

        try(Connection c=DBUtil.getConnection();Statement s=c.createStatement();)
        {
            String sql="select * from address where id="+id;
            ResultSet rs=s.executeQuery(sql);

            if(rs.next())
            {
                bean=new Address();
                String name=rs.getString(2);
                bean.setName(name);
                bean.setId(id);
            }
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        return bean;
    }

    public List<Address> list(int i, short maxValue)
    {
        return list(0,Short.MAX_VALUE);
    }

    public List<Address> list(int start,int count)
    {
        List<Address> beans=new ArrayList<Address>();
        String sql="select * from address order by id desc limit ?,?";
        try(Connection c=DBUtil.getConnection();PreparedStatement ps=c.prepareStatement(sql);)
        {
            ps.setInt(1,start);
            ps.setInt(2,count);
            ResultSet rs=ps.executeQuery();

            while(rs.next())
            {
                Address bean=new Address();
                int id=rs.getInt(1);
                String name=rs.getString(2);
                bean.setId(id);
                bean.setName(name);
                beans.add(bean);
            }
            catch(SQLException e)
            {
                e.printStackTrace();
            }
            return beans;
        }
    }

}
