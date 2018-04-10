package lora.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import lora.bean.Appliance;
import lora.bean.Img;
import lora.dao.ApplianceDAO;
import lora.util.DBUtil;

public class ImgDAO {

    public static final String type_single = "type_single";
    public static final String type_detail = "type_detail";


    public int getTotal() {
        int total = 0;
        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select count(*) from Img";

            ResultSet rs = s.executeQuery(sql);
            while (rs.next()) {
                total = rs.getInt(1);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return total;
    }

    public void add(Img bean) {
        String sql = "insert into Img values(null,?,?)";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {
            ps.setInt(1, bean.getAppliance().getId());
            ps.setString(2, bean.getType());
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

    public void update(Img bean) {

    }

    public void delete(int id) {

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "delete from Img where id = " + id;

            s.execute(sql);

        } catch (SQLException e) {

            e.printStackTrace();
        }
    }

    public Img get(int id) {
        Img bean = new Img();
        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select * from Img where id = " + id;

            ResultSet rs = s.executeQuery(sql);

            if (rs.next()) {
                int pid = rs.getInt("pid");
                String type = rs.getString("type");
                Appliance appliance = new ApplianceDAO().get(pid);
                bean.setAppliance(appliance);
                bean.setType(type);
                bean.setId(id);
            }

        } catch (SQLException e) {

            e.printStackTrace();
        }
        return bean;
    }

    public List<Img> list(Appliance a, String type) {
        return list(a, type,0, Short.MAX_VALUE);
    }

    public List<Img> list(Appliance a, String type, int start, int count) {
        List<Img> beans = new ArrayList<Img>();

        String sql = "select * from Img where pid =? and type =? order by id desc limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setInt(1, a.getId());
            ps.setString(2, type);

            ps.setInt(3, start);
            ps.setInt(4, count);



            ResultSet rs = ps.executeQuery();

            while (rs.next()) {

                Img bean = new Img();
                int id = rs.getInt(1);


                bean.setAppliance(a);
                bean.setType(type);
                bean.setId(id);

                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }

}