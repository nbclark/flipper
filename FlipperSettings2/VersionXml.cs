using System;
using System.Xml.Serialization;

namespace MobileSRC.GScroll
{
    [XmlRoot("application")]
    public class VersionXml
    {
        [XmlElement("name")]
        public string Name
        {
            get;
            set;
        }
        [XmlElement("version")]
        public double Version
        {
            get;
            set;
        }
        [XmlElement("description")]
        public string Description
        {
            get;
            set;
        }
        [XmlElement("versionurl")]
        public string VersionUrl
        {
            get;
            set;
        }
        [XmlElement("caburl")]
        public string CabUrl
        {
            get;
            set;
        }
    }
}
