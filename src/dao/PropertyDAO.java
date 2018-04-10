package dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import bean.Appliance;
import bean.Room;
import bean.Property;
import dbset.DBUtil;

public class PropertyDAO {

    public int getTotal() {
        int total = 0;
        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select count(*) from Property";

            ResultSet rs = s.executeQuery(sql);
            while (rs.next()) {
                total = rs.getInt(1);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return total;
    }

    public void add(Property bean) {

        String sql = "insert into Property values(null,?,?,?)";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setInt(1, bean.getAppliance().getId());
            ps.setInt(2, bean.getRoom().getId());
            ps.setString(3, bean.getUseapp());
            ps.execute();

            ResultSet rs = ps.getGeneratedKeys();
            if (rs.next()) {
                int id = rs.getInt(1);
                bean.setId(id);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
    }

    public void update(Property bean) {

        String sql = "update Property set pid= ?, tid=?, useapp=?  where id = ?";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {
            ps.setInt(1, bean.getAppliance().getId());
            ps.setInt(2, bean.getRoom().getId());
            ps.setString(3, bean.getUseapp());
            ps.setInt(4, bean.getId());
            ps.execute();

        } catch (SQLException e) {

            e.printStackTrace();
        }

    }

    public void delete(int id) {

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "delete from Property where id = " + id;

            s.execute(sql);

        } catch (SQLException e) {

            e.printStackTrace();
        }
    }

    public Property get(int id) {
        Property bean = new Property();

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select * from Property where id = " + id;

            ResultSet rs = s.executeQuery(sql);

            if (rs.next()) {
                int pid = rs.getInt("pid");
                int tid = rs.getInt("tid");
                String useapp = rs.getString("useapp");

                Appliance appliance = new ApplianceDAO().get(pid);
                Room room = new RoomDAO().get(tid);
                bean.setAppliance(appliance);
                bean.setRoom(room);
                bean.setUseapp(useapp);
                bean.setId(id);
            }

        } catch (SQLException e) {

            e.printStackTrace();
        }
        return bean;
    }
    public Property get(int tid, int pid ) {
        Property bean = null;

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select * from Property where tid = " + tid + " and pid = " + pid;

            ResultSet rs = s.executeQuery(sql);

            if (rs.next()) {
                bean= new Property();
                int id = rs.getInt("id");

                String useapp = rs.getString("useapp");

                Appliance appliance = new ApplianceDAO().get(pid);
                Room room = new RoomDAO().get(tid);
                bean.setAppliance(appliance);
                bean.setRoom(room);
                bean.setUseapp(useapp);
                bean.setId(id);
            }

        } catch (SQLException e) {

            e.printStackTrace();
        }
        return bean;
    }

    public List<Property> list() {
        return list(0, Short.MAX_VALUE);
    }

    public List<Property> list(int start, int count) {
        List<Property> beans = new ArrayList<Property>();

        String sql = "select * from Property order by id desc limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setInt(1, start);
            ps.setInt(2, count);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Property bean = new Property();
                int id = rs.getInt(1);

                int pid = rs.getInt("pid");
                int tid = rs.getInt("tid");
                String useapp = rs.getString("useapp");

                Appliance appliance = new ApplianceDAO().get(pid);
                Room room = new RoomDAO().get(tid);
                bean.setAppliance(appliance);
                bean.setRoom(room);
                bean.setUseapp(useapp);
                bean.setId(id);
                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }

    public void init(Appliance p) {
        List<Room> pts= new RoomDAO().list(p.getAddress().getId());

        for (Room pt: pts) {
            Property pv = get(pt.getId(),p.getId());
            if(null==pv){
                pv = new Property();
                pv.setAppliance(p);
                pv.setRoom(pt);
                this.add(pv);
            }
        }
    }

    public List<Property> list(int pid) {
        List<Property> beans = new ArrayList<Property>();

        String sql = "select * from Property where pid = ? order by tid desc";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setInt(1, pid);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Property bean = new Property();
                int id = rs.getInt(1);

                int tid = rs.getInt("tid");
                String useapp = rs.getString("value");

                Appliance appliance = new ApplianceDAO().get(pid);
                Room room = new RoomDAO().get(tid);
                bean.setAppliance(appliance);
                bean.setRoom(room);
                bean.setUseapp(useapp);
                bean.setId(id);
                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }

}