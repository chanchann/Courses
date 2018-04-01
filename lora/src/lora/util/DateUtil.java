package lora.util;

public class DateUtil
{
    public static java.sql.Timestamp dateToTimestamp(java.util.Date d)
    {
        if(null==d)
            return null;
        return new java.sql.Timestamp(d.getTime());
    }

    public static java.util.Date timestampToDate(java.sql.Date t)
    {
        if(null==t)
            return null;
        return new java.util.Date(t.getTime());
    }
}
