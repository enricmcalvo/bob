#!/usr/bin/env python
# vim: set fileencoding=utf-8 :
# Laurent El Shafey <laurent.el-shafey@idiap.ch>

"""Table models and functionality for the Multi-PIE database.
"""

import sqlalchemy
from sqlalchemy import Column, Integer, String, ForeignKey, or_, and_
from sqlalchemy.orm import relationship, backref
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()

class Client(Base):
  __tablename__ = 'client'

  id = Column(Integer, primary_key=True)
  sgroup = Column(sqlalchemy.Enum('dev', 'eval', 'world'))
  birthyear = Column(Integer)
  gender = Column(sqlalchemy.Enum('m','f'))

  def __init__(self, id, group, birthyear, gender):
    self.id = id
    self.sgroup = group
    self.birthyear = birthyear
    self.gender = gender

  def __repr__(self):
    return "<Client('%d', '%s', '%s', '%s', '%s')>" % (self.id, self.sgroup, self.birthyear, self.gender)

class File(Base):
  __tablename__ = 'file'

  id = Column(Integer, primary_key=True)
  client_id = Column(Integer, ForeignKey('client.id')) # for SQL
  path = Column(String(100), unique=True)
  session_id = Column(Integer)#, ForeignKey('session.id'))
  recording_id = Column(Integer)
  img_type = Column(sqlalchemy.Enum('multiview', 'highres'))

  # for Python
  client = relationship("Client", backref=backref("client_file"))

  def __init__(self, client_id, path, session_id, recording_id, img_type):
    self.client_id = client_id
    self.path = path
    self.session_id = session_id
    self.recording_id = recording_id
    self.img_type = img_type

  def __repr__(self):
    print "<File('%s')>" % self.path

class FileMultiview(Base):
  __tablename__ = 'fileMultiview'

  id = Column(Integer, ForeignKey('file.id'), primary_key=True) # for SQL
  camera_id = Column(String(6))
  shot_id = Column(Integer)

  # for Python
  file = relationship("File", backref=backref("file_fileMultiview"))

  def __init__(self, file_id, camera_id, shot_id):
    self.id = file_id
    self.camera_id = camera_id
    self.shot_id = shot_id

  def __repr__(self):
    print "<FileMultiview('%d','%d')>" % self.camera_id, self.shot_id

class Expression(Base):
  __tablename__ = 'expression'
  
  id = Column(Integer, primary_key=True)
  name = Column(sqlalchemy.Enum('neutral', 'smile', 'surprise', 'squint', 'disgust', 'scream'))
  img_type = Column(sqlalchemy.Enum('multiview', 'highres'))
  session_id = Column(Integer)
  recording_id = Column(Integer)

  def __init__(self, name, img_type, session_id, recording_id):
    self.name = name
    self.img_type = img_type
    self.session_id = session_id
    self.recording_id = recording_id

  def __repr__(self):
    return "<Expression('%s', '%s', '%d', '%d')>" % (self.name, self.img_type, self.session_id, self.recording_id)

class ProtocolName(Base):
  __tablename__ = 'protocolName'
  
  name = Column(String(6), primary_key=True)

  def __init__(self, name):
    self.name = name

  def __repr__(self):
    return "<ProtocolName('%s')>" % (self.name)

class Protocol(Base):
  __tablename__ = 'protocol'

  id = Column(Integer, primary_key=True)
  name = Column(String(6), ForeignKey('protocolName.name'))
  sgroup = Column(sqlalchemy.Enum('dev','eval','world')) # DO NOT USE GROUP (LIKELY KEYWORD)
  purpose = Column(sqlalchemy.Enum('enrol', 'probe', 'probeImpostor', 'world'))
  session_id = Column(Integer)#, ForeignKey('session.id'))
  recording_id = Column(Integer)
  img_type = Column(sqlalchemy.Enum('multiview', 'highres'))

  # for Python
  protocol = relationship("ProtocolName", backref=backref("protocolName_protocol"))

  def __init__(self, name, group, purpose, session_id, recording_id):
    self.name = name
    self.sgroup = group
    self.purpose = purpose
    self.session_id = session_id
    self.recording_id = recording_id

  def __repr__(self):
    return "<ProtocolMultiview('%s', '%s', '%s', '%d', '%d')>" % (self.name, self.sgroup, self.purpose, self.session_id, self.recording_id)

class ProtocolMultiview(Base):
  __tablename__ = 'protocolMultiview'

  id = Column(Integer, ForeignKey('protocol.id'), primary_key=True) # for SQL
  camera_id = Column(String(6))
  shot_id = Column(Integer)

  # for Python
  protocol = relationship("Protocol", backref=backref("protocol_protocolMultiview"))

  def __init__(self, protocol_id, camera_id, shot_id):
    self.id = protocol_id
    self.camera_id = camera_id
    self.shot_id = shot_id

  def __repr__(self):
    print "<ProtocolMultiview('%d','%d')>" % (self.camera_id, self.shot_id)

class ProtocolProbeImpostorId(Base):
  __tablename__ = 'ProtocolProbeImpostorId'

  from sqlalchemy import Enum # import locally (not supported by old 10.04 Ubuntu package)
  id = Column(Integer, primary_key=True)
  name = Column(String(6), ForeignKey('protocolName.name'))
  claimed_id = Column(Integer, ForeignKey('client.id')) # for SQL
  real_id = Column(Integer, ForeignKey('client.id')) # for SQL

  def __init__(self, name, claimed_id, real_id, group):
    self.name = name
    self.claimed_id = claimed_id
    self.real_id = real_id

  def __repr__(self):
    return "<ProtocolProbeImpostorId('%s', '%d', '%d')>" % (self.name, self.claimed_id, self.real_id)
