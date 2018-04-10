package dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import bean.Address;
import bean.Room;
import dao.AddressDAO;
import dbset.DBUtil;

public class RoomDAO
{
    public int getTotal(int cid)
    {
        int total = 0;
        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select count(*) from Room where cid =" + cid;

            ResultSet rs = s.executeQuery(sql);
            while (rs.next()) {
                total = rs.getInt(1);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return total;
    }

    public void add(Room bean)
    {
        String sql = "insert into Room values(null,?,?)";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);)
        {
            ps.setInt(1, bean.getAddress().getId());
            ps.setString(2, bean.getName());
            ps.execute();

            ResultSet rs = ps.getGeneratedKeys();
            if (rs.next())
            {
                int id = rs.getInt(1);
                bean.setId(id);
            }
        }
        catch (SQLException e)
        {

            e.printStackTrace();
        }
    }

    public void update(Room bean) {

        String sql = "update Room set cid= ?, name=? where id = ?";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);)
        {
            ps.setInt(1, bean.getAddress().getId());
            ps.setString(2, bean.getName());
            ps.setInt(3, bean.getId());
            ps.execute();
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }

    }

    public void delete(int id)
    {

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();)
        {
            String sql = "delete from Room where id = " + id;
            s.execute(sql);
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
    }

    public Room get(String name, int cid) {
        Room bean =null;
        String sql = "select * from Room where name = ? and cid = ?";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql))
        {
            ps.setString(1, name);
            ps.setInt(2, cid);
            ResultSet rs = ps.executeQuery();

            if (rs.next()) {
                int id = rs.getInt("id");
                bean = new Room();
                bean.setName(name);
                Address address = new AddressDAO().get(cid);
                bean.setAddress(address);
                bean.setId(id);
            }

        }
        catch (SQLException e) {

            e.printStackTrace();
        }
        return bean;
    }
    public Room get(int id) {
        Room bean = new Room();

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();)
        {
            String sql = "select * from Room where id = " + id;
            ResultSet rs = s.executeQuery(sql);
            if (rs.next()) {
                String name = rs.getString("name");
                int cid = rs.getInt("cid");
                bean.setName(name);
                Address address = new AddressDAO().get(cid);
                bean.setAddress(address);
                bean.setId(id);
            }

        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
        return bean;
    }

    public List<Room> list(int cid)
    {
        return list(cid, 0, Short.MAX_VALUE);
    }

    public List<Room> list(int cid, int start, int count)
    {
        List<Room> beans = new ArrayList<Room>();

        String sql = "select * from Room where cid = ? order by id desc limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);)
        {
            ps.setInt(1, cid);
            ps.setInt(2, start);
            ps.setInt(3, count);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Room bean = new Room();
                int id = rs.getInt(1);
                String name = rs.getString("name");
                bean.setName(name);
                Address address = new AddressDAO().get(cid);
                bean.setAddress(address);
                bean.setId(id);

                beans.add(bean);
            }
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
        return beans;
    }

}