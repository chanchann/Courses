package bean;

import java.util.Date;
import java.util.List;

public class Appliance
{
    private String name;
    private String remark;
    private Date createDate;
    private Address address;
    private int id;
    private Img firstImg;
    private List<Img> images;
    private List<Img> singleImages;
    private List<Img> detailImages;
    private int reviewCount;

    public String getName()
    {
        return name;
    }

    public void setName(String name)
    {
        this.name = name;
    }

    public String getRemark()
    {
        return remark;
    }

    public void setRemark(String remark)
    {
        this.remark = remark;
    }

    public Date getCreateDate()
    {
        return createDate;
    }

    public void setCreateDate(Date createDate)
    {
        this.createDate = createDate;
    }

    public Address getAddress()
    {
        return address;
    }

    public void setAddress(Address address)
    {
        this.address = address;
    }

    public int getId()
    {
        return id;
    }

    public void setId(int id)
    {
        this.id = id;
    }

    public Img getFirstImg()
    {
        return firstImg;
    }

    public void setFirstImg(Img firstImg)
    {
        this.firstImg = firstImg;
    }

    public List<Img> getImages()
    {
        return images;
    }

    public void setImages(List<Img> images)
    {
        this.images = images;
    }

    public List<Img> getSingleImages()
    {
        return singleImages;
    }

    public void setSingleImages(List<Img> singleImages)
    {
        this.singleImages = singleImages;
    }

    public List<Img> getDetailImages()
    {
        return detailImages;
    }

    public void setDetailImages(List<Img> detailImages)
    {
        this.detailImages = detailImages;
    }

    public int getReviewCount()
    {
        return reviewCount;
    }

    public void setReviewCount(int reviewCount)
    {
        this.reviewCount = reviewCount;
    }
}
