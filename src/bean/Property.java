package bean;

public class Property
{
    private String useapp;
    private String light;
    private String fire;
    private String pm25;
    private String humidity;
    private String temperature;
    private Appliance appliance;
    private Room room;
    private int id;

    public String getUseapp()
    {
        return useapp;
    }

    public void setUseapp(String useapp)
    {
        this.useapp = useapp;
    }

    public String getLight()
    {
        return light;
    }

    public void setLight(String light)
    {
        this.light = light;
    }

    public String getFire()
    {
        return fire;
    }

    public void setFire(String fire)
    {
        this.fire = fire;
    }

    public String getPm25()
    {
        return pm25;
    }

    public void setPm25(String pm25)
    {
        this.pm25 = pm25;
    }

    public String getHumidity()
    {
        return humidity;
    }

    public void setHumidity(String humidity)
    {
        this.humidity = humidity;
    }

    public String getTemperature()
    {
        return temperature;
    }

    public void setTemperature(String temperature)
    {
        this.temperature = temperature;
    }

    public Appliance getAppliance()
    {
        return appliance;
    }

    public void setAppliance(Appliance appliance)
    {
        this.appliance = appliance;
    }

    public Room getRoom()
    {
        return room;
    }

    public void setRoom(Room room)
    {
        this.room = room;
    }

    public int getId()
    {
        return id;
    }

    public void setId(int id)
    {
        this.id = id;
    }
}
