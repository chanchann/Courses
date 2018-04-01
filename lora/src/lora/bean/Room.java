package lora.bean;

public class Room
{
    private int id;
    private Address address;
    private String roomName;

    public int getId()
    {
        return id;
    }

    public void setId(int id)
    {
        this.id = id;
    }

    public Address getAddress()
    {
        return address;
    }

    public void setAddress(Address address)
    {
        this.address = address;
    }

    public String getName()
    {
        return roomName;
    }

    public void setName(String name)
    {
        this.roomName = name;
    }
}
