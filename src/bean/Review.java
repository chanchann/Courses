package bean;

import java.util.Date;

public class Review
{
    private String content;
    private Date createDate;
    private User user;
    private Appliance appliance;
    private int id;

    public String getContent()
    {
        return content;
    }

    public void setContent(String cpntent)
    {
        this.content = content;

    }

    public Date getCreateDate()
    {
        return createDate;
    }

    public void setCreateDate(Date createDate)
    {
        this.createDate = createDate;
    }

    public User getUser()
    {
        return user;
    }

    public void setUser(User user)
    {
        this.user = user;
    }

    public Appliance getAppliance()
    {
        return appliance;
    }

    public void setAppliance(Appliance appliance)
    {
        this.appliance = appliance;
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
