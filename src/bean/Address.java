package bean;

import java.util.List;

public class Address
{
    private int id;
    private String addressName;
    List<Appliance> appliance;
    List<List<Appliance>> applianceByRow;

    public int getId()
    {
        return id;
    }

    public void setId(int id)
    {
        this.id = id;
    }

    public String getName()
    {
        return addressName;
    }

    public void setName(String name)
    {
        this.addressName = name;
    }

    public List<Appliance> getAppliance()
    {
        return appliance;
    }

    public void setAppliance(List<Appliance> appliance)
    {
        this.appliance = appliance;
    }

    public List<List<Appliance>> getApplianceByRow()
    {
        return applianceByRow;
    }

    public void setApplianceByRow(List<List<Appliance>> applianceByRow)
    {
        this.applianceByRow = applianceByRow;
    }

}
