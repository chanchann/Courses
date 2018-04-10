package lora.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import lora.bean.Address;
import lora.bean.Appliance;
import lora.bean.Img;
import lora.util.DBUtil;
import lora.util.DateUtil;

public class ApplianceDAO {

    public int getTotal(int cid) {
        int total = 0;
        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select count(*) from Appliance where cid = " + cid;

            ResultSet rs = s.executeQuery(sql);
            while (rs.next()) {
                total = rs.getInt(1);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return total;
    }

    public void add(Appliance bean) {

        String sql = "insert into Appliance values(null,?,?,?,?)";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setString(1, bean.getName());
            ps.setString(2, bean.getRemark());
            ps.setInt(3, bean.getAddress().getId());
            ps.setTimestamp(4, DateUtil.dateToTimestamp(bean.getDate()));
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

    public void update(Appliance bean) {

        String sql = "update Appliance set name= ?, remark=?, cid = ?, Date=? where id = ?";
        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setString(1, bean.getName());
            ps.setString(2, bean.getRemark());
            ps.setInt(3, bean.getAddress().getId());
            ps.setTimestamp(4, DateUtil.dateToTimestamp(bean.getDate()));
            ps.setInt(5, bean.getId());
            ps.execute();

        } catch (SQLException e) {

            e.printStackTrace();
        }

    }

    public void delete(int id) {

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "delete from Appliance where id = " + id;

            s.execute(sql);

        } catch (SQLException e) {

            e.printStackTrace();
        }
    }

    public Appliance get(int id) {
        Appliance bean = new Appliance();

        try (Connection c = DBUtil.getConnection(); Statement s = c.createStatement();) {

            String sql = "select * from Appliance where id = " + id;

            ResultSet rs = s.executeQuery(sql);

            if (rs.next()) {

                String name = rs.getString("name");
                String remark = rs.getString("remark");
                int cid = rs.getInt("cid");
                Date date = DateUtil.timestampToDate(rs.getTimestamp("Date"));

                bean.setName(name);
                bean.setRemark(remark);
                Address address = new AddressDAO().get(cid);
                bean.setAddress(address);
                bean.setDate(date);
                bean.setId(id);
                setFirstImg(bean);
            }

        } catch (SQLException e) {

            e.printStackTrace();
        }
        return bean;
    }

    public List<Appliance> list(int cid) {
        return list(cid,0, Short.MAX_VALUE);
    }

    public List<Appliance> list(int cid, int start, int count) {
        List<Appliance> beans = new ArrayList<Appliance>();
        Address address = new AddressDAO().get(cid);
        String sql = "select * from Appliance where cid = ? order by id desc limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {
            ps.setInt(1, cid);
            ps.setInt(2, start);
            ps.setInt(3, count);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Appliance bean = new Appliance();
                int id = rs.getInt(1);
                String name = rs.getString("name");
                String remark = rs.getString("remark");
                Date date = DateUtil.timestampToDate( rs.getTimestamp("Date"));

                bean.setName(name);
                bean.setRemark(remark);
                bean.setDate(date);
                bean.setId(id);
                bean.setAddress(address);
                setFirstImg(bean);
                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }
    public List<Appliance> list() {
        return list(0,Short.MAX_VALUE);
    }
    public List<Appliance> list(int start, int count) {
        List<Appliance> beans = new ArrayList<Appliance>();

        String sql = "select * from Appliance limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {

            ps.setInt(1, start);
            ps.setInt(2, count);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Appliance bean = new Appliance();
                int id = rs.getInt(1);
                int cid = rs.getInt("cid");
                String name = rs.getString("name");
                String remark = rs.getString("remark");

                Date date = DateUtil.timestampToDate( rs.getTimestamp("Date"));

                bean.setName(name);
                bean.setRemark(remark);
                bean.setDate(date);
                bean.setId(id);

                Address Address = new AddressDAO().get(cid);
                bean.setAddress(Address);
                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }

    public void fill(List<Address> cs) {
        for (Address c : cs)
            fill(c);
    }
    public void fill(Address c) {
        List<Appliance> ps = this.list(c.getId());
        c.setAppliance(ps);
    }

    public void fillByRow(List<Address> cs) {
        int ApplianceNumberEachRow = 8;
        for (Address c : cs) {
            List<Appliance> Appliances =  c.getAppliance();
            List<List<Appliance>> AppliancesByRow =  new ArrayList<>();
            for (int i = 0; i < Appliances.size(); i+=ApplianceNumberEachRow) {
                int size = i+ApplianceNumberEachRow;
                size= size>Appliances.size()?Appliances.size():size;
                List<Appliance> AppliancesOfEachRow =Appliances.subList(i, size);
                AppliancesByRow.add(AppliancesOfEachRow);
            }
            c.setApplianceByRow(AppliancesByRow);
        }
    }

    public void setFirstImg(Appliance p) {
        List<Img> pis= new ImgDAO().list(p, ImgDAO.type_single);
        if(!pis.isEmpty())
            p.setFirstImg(pis.get(0));
    }


    public List<Appliance> search(String keyword, int start, int count) {
        List<Appliance> beans = new ArrayList<Appliance>();

        if(null==keyword||0==keyword.trim().length())
            return beans;
        String sql = "select * from Appliance where name like ? limit ?,? ";

        try (Connection c = DBUtil.getConnection(); PreparedStatement ps = c.prepareStatement(sql);) {
            ps.setString(1, "%"+keyword.trim()+"%");
            ps.setInt(2, start);
            ps.setInt(3, count);

            ResultSet rs = ps.executeQuery();

            while (rs.next()) {
                Appliance bean = new Appliance();
                int id = rs.getInt(1);
                int cid = rs.getInt("cid");
                String name = rs.getString("name");
                String remark = rs.getString("remark");
                Date date = DateUtil.timestampToDate( rs.getTimestamp("Date"));

                bean.setName(name);
                bean.setRemark(remark);
                bean.setDate(date);
                bean.setId(id);

                Address address = new AddressDAO().get(cid);
                bean.setAddress(address);
                setFirstImg(bean);
                beans.add(bean);
            }
        } catch (SQLException e) {

            e.printStackTrace();
        }
        return beans;
    }
}

