// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: FkPbValue.proto

package com.alimin.fk.pb;

public final class FkPbValueOuterClass {
  private FkPbValueOuterClass() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface FkPbValueOrBuilder extends
      // @@protoc_insertion_point(interface_extends:com.alimin.fk.pb.FkPbValue)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>int32 int32Val = 1;</code>
     * @return The int32Val.
     */
    int getInt32Val();

    /**
     * <code>int64 int64Val = 2;</code>
     * @return The int64Val.
     */
    long getInt64Val();

    /**
     * <code>float floatVal = 3;</code>
     * @return The floatVal.
     */
    float getFloatVal();

    /**
     * <code>double doubleVal = 4;</code>
     * @return The doubleVal.
     */
    double getDoubleVal();

    /**
     * <code>string strVal = 5;</code>
     * @return The strVal.
     */
    java.lang.String getStrVal();
    /**
     * <code>string strVal = 5;</code>
     * @return The bytes for strVal.
     */
    com.google.protobuf.ByteString
        getStrValBytes();
  }
  /**
   * Protobuf type {@code com.alimin.fk.pb.FkPbValue}
   */
  public static final class FkPbValue extends
      com.google.protobuf.GeneratedMessageV3 implements
      // @@protoc_insertion_point(message_implements:com.alimin.fk.pb.FkPbValue)
      FkPbValueOrBuilder {
  private static final long serialVersionUID = 0L;
    // Use FkPbValue.newBuilder() to construct.
    private FkPbValue(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
      super(builder);
    }
    private FkPbValue() {
      strVal_ = "";
    }

    @java.lang.Override
    @SuppressWarnings({"unused"})
    protected java.lang.Object newInstance(
        UnusedPrivateParameter unused) {
      return new FkPbValue();
    }

    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
    getUnknownFields() {
      return this.unknownFields;
    }
    private FkPbValue(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      this();
      if (extensionRegistry == null) {
        throw new java.lang.NullPointerException();
      }
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
          com.google.protobuf.UnknownFieldSet.newBuilder();
      try {
        boolean done = false;
        while (!done) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              done = true;
              break;
            case 8: {

              int32Val_ = input.readInt32();
              break;
            }
            case 16: {

              int64Val_ = input.readInt64();
              break;
            }
            case 29: {

              floatVal_ = input.readFloat();
              break;
            }
            case 33: {

              doubleVal_ = input.readDouble();
              break;
            }
            case 42: {
              java.lang.String s = input.readStringRequireUtf8();

              strVal_ = s;
              break;
            }
            default: {
              if (!parseUnknownField(
                  input, unknownFields, extensionRegistry, tag)) {
                done = true;
              }
              break;
            }
          }
        }
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        throw e.setUnfinishedMessage(this);
      } catch (java.io.IOException e) {
        throw new com.google.protobuf.InvalidProtocolBufferException(
            e).setUnfinishedMessage(this);
      } finally {
        this.unknownFields = unknownFields.build();
        makeExtensionsImmutable();
      }
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return com.alimin.fk.pb.FkPbValueOuterClass.internal_static_com_alimin_fk_pb_FkPbValue_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return com.alimin.fk.pb.FkPbValueOuterClass.internal_static_com_alimin_fk_pb_FkPbValue_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.class, com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.Builder.class);
    }

    public static final int INT32VAL_FIELD_NUMBER = 1;
    private int int32Val_;
    /**
     * <code>int32 int32Val = 1;</code>
     * @return The int32Val.
     */
    @java.lang.Override
    public int getInt32Val() {
      return int32Val_;
    }

    public static final int INT64VAL_FIELD_NUMBER = 2;
    private long int64Val_;
    /**
     * <code>int64 int64Val = 2;</code>
     * @return The int64Val.
     */
    @java.lang.Override
    public long getInt64Val() {
      return int64Val_;
    }

    public static final int FLOATVAL_FIELD_NUMBER = 3;
    private float floatVal_;
    /**
     * <code>float floatVal = 3;</code>
     * @return The floatVal.
     */
    @java.lang.Override
    public float getFloatVal() {
      return floatVal_;
    }

    public static final int DOUBLEVAL_FIELD_NUMBER = 4;
    private double doubleVal_;
    /**
     * <code>double doubleVal = 4;</code>
     * @return The doubleVal.
     */
    @java.lang.Override
    public double getDoubleVal() {
      return doubleVal_;
    }

    public static final int STRVAL_FIELD_NUMBER = 5;
    private volatile java.lang.Object strVal_;
    /**
     * <code>string strVal = 5;</code>
     * @return The strVal.
     */
    @java.lang.Override
    public java.lang.String getStrVal() {
      java.lang.Object ref = strVal_;
      if (ref instanceof java.lang.String) {
        return (java.lang.String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        strVal_ = s;
        return s;
      }
    }
    /**
     * <code>string strVal = 5;</code>
     * @return The bytes for strVal.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString
        getStrValBytes() {
      java.lang.Object ref = strVal_;
      if (ref instanceof java.lang.String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        strVal_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }

    private byte memoizedIsInitialized = -1;
    @java.lang.Override
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized == 1) return true;
      if (isInitialized == 0) return false;

      memoizedIsInitialized = 1;
      return true;
    }

    @java.lang.Override
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (int32Val_ != 0) {
        output.writeInt32(1, int32Val_);
      }
      if (int64Val_ != 0L) {
        output.writeInt64(2, int64Val_);
      }
      if (java.lang.Float.floatToRawIntBits(floatVal_) != 0) {
        output.writeFloat(3, floatVal_);
      }
      if (java.lang.Double.doubleToRawLongBits(doubleVal_) != 0) {
        output.writeDouble(4, doubleVal_);
      }
      if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(strVal_)) {
        com.google.protobuf.GeneratedMessageV3.writeString(output, 5, strVal_);
      }
      unknownFields.writeTo(output);
    }

    @java.lang.Override
    public int getSerializedSize() {
      int size = memoizedSize;
      if (size != -1) return size;

      size = 0;
      if (int32Val_ != 0) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, int32Val_);
      }
      if (int64Val_ != 0L) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt64Size(2, int64Val_);
      }
      if (java.lang.Float.floatToRawIntBits(floatVal_) != 0) {
        size += com.google.protobuf.CodedOutputStream
          .computeFloatSize(3, floatVal_);
      }
      if (java.lang.Double.doubleToRawLongBits(doubleVal_) != 0) {
        size += com.google.protobuf.CodedOutputStream
          .computeDoubleSize(4, doubleVal_);
      }
      if (!com.google.protobuf.GeneratedMessageV3.isStringEmpty(strVal_)) {
        size += com.google.protobuf.GeneratedMessageV3.computeStringSize(5, strVal_);
      }
      size += unknownFields.getSerializedSize();
      memoizedSize = size;
      return size;
    }

    @java.lang.Override
    public boolean equals(final java.lang.Object obj) {
      if (obj == this) {
       return true;
      }
      if (!(obj instanceof com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue)) {
        return super.equals(obj);
      }
      com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue other = (com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue) obj;

      if (getInt32Val()
          != other.getInt32Val()) return false;
      if (getInt64Val()
          != other.getInt64Val()) return false;
      if (java.lang.Float.floatToIntBits(getFloatVal())
          != java.lang.Float.floatToIntBits(
              other.getFloatVal())) return false;
      if (java.lang.Double.doubleToLongBits(getDoubleVal())
          != java.lang.Double.doubleToLongBits(
              other.getDoubleVal())) return false;
      if (!getStrVal()
          .equals(other.getStrVal())) return false;
      if (!unknownFields.equals(other.unknownFields)) return false;
      return true;
    }

    @java.lang.Override
    public int hashCode() {
      if (memoizedHashCode != 0) {
        return memoizedHashCode;
      }
      int hash = 41;
      hash = (19 * hash) + getDescriptor().hashCode();
      hash = (37 * hash) + INT32VAL_FIELD_NUMBER;
      hash = (53 * hash) + getInt32Val();
      hash = (37 * hash) + INT64VAL_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          getInt64Val());
      hash = (37 * hash) + FLOATVAL_FIELD_NUMBER;
      hash = (53 * hash) + java.lang.Float.floatToIntBits(
          getFloatVal());
      hash = (37 * hash) + DOUBLEVAL_FIELD_NUMBER;
      hash = (53 * hash) + com.google.protobuf.Internal.hashLong(
          java.lang.Double.doubleToLongBits(getDoubleVal()));
      hash = (37 * hash) + STRVAL_FIELD_NUMBER;
      hash = (53 * hash) + getStrVal().hashCode();
      hash = (29 * hash) + unknownFields.hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseDelimitedWithIOException(PARSER, input);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input);
    }
    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    @java.lang.Override
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder() {
      return DEFAULT_INSTANCE.toBuilder();
    }
    public static Builder newBuilder(com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue prototype) {
      return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
    }
    @java.lang.Override
    public Builder toBuilder() {
      return this == DEFAULT_INSTANCE
          ? new Builder() : new Builder().mergeFrom(this);
    }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code com.alimin.fk.pb.FkPbValue}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:com.alimin.fk.pb.FkPbValue)
        com.alimin.fk.pb.FkPbValueOuterClass.FkPbValueOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return com.alimin.fk.pb.FkPbValueOuterClass.internal_static_com_alimin_fk_pb_FkPbValue_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return com.alimin.fk.pb.FkPbValueOuterClass.internal_static_com_alimin_fk_pb_FkPbValue_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.class, com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.Builder.class);
      }

      // Construct using com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.newBuilder()
      private Builder() {
        maybeForceBuilderInitialization();
      }

      private Builder(
          com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
        super(parent);
        maybeForceBuilderInitialization();
      }
      private void maybeForceBuilderInitialization() {
        if (com.google.protobuf.GeneratedMessageV3
                .alwaysUseFieldBuilders) {
        }
      }
      @java.lang.Override
      public Builder clear() {
        super.clear();
        int32Val_ = 0;

        int64Val_ = 0L;

        floatVal_ = 0F;

        doubleVal_ = 0D;

        strVal_ = "";

        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return com.alimin.fk.pb.FkPbValueOuterClass.internal_static_com_alimin_fk_pb_FkPbValue_descriptor;
      }

      @java.lang.Override
      public com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue getDefaultInstanceForType() {
        return com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.getDefaultInstance();
      }

      @java.lang.Override
      public com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue build() {
        com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue buildPartial() {
        com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue result = new com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue(this);
        result.int32Val_ = int32Val_;
        result.int64Val_ = int64Val_;
        result.floatVal_ = floatVal_;
        result.doubleVal_ = doubleVal_;
        result.strVal_ = strVal_;
        onBuilt();
        return result;
      }

      @java.lang.Override
      public Builder clone() {
        return super.clone();
      }
      @java.lang.Override
      public Builder setField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          java.lang.Object value) {
        return super.setField(field, value);
      }
      @java.lang.Override
      public Builder clearField(
          com.google.protobuf.Descriptors.FieldDescriptor field) {
        return super.clearField(field);
      }
      @java.lang.Override
      public Builder clearOneof(
          com.google.protobuf.Descriptors.OneofDescriptor oneof) {
        return super.clearOneof(oneof);
      }
      @java.lang.Override
      public Builder setRepeatedField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          int index, java.lang.Object value) {
        return super.setRepeatedField(field, index, value);
      }
      @java.lang.Override
      public Builder addRepeatedField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          java.lang.Object value) {
        return super.addRepeatedField(field, value);
      }
      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue) {
          return mergeFrom((com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue other) {
        if (other == com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue.getDefaultInstance()) return this;
        if (other.getInt32Val() != 0) {
          setInt32Val(other.getInt32Val());
        }
        if (other.getInt64Val() != 0L) {
          setInt64Val(other.getInt64Val());
        }
        if (other.getFloatVal() != 0F) {
          setFloatVal(other.getFloatVal());
        }
        if (other.getDoubleVal() != 0D) {
          setDoubleVal(other.getDoubleVal());
        }
        if (!other.getStrVal().isEmpty()) {
          strVal_ = other.strVal_;
          onChanged();
        }
        this.mergeUnknownFields(other.unknownFields);
        onChanged();
        return this;
      }

      @java.lang.Override
      public final boolean isInitialized() {
        return true;
      }

      @java.lang.Override
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue parsedMessage = null;
        try {
          parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          parsedMessage = (com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue) e.getUnfinishedMessage();
          throw e.unwrapIOException();
        } finally {
          if (parsedMessage != null) {
            mergeFrom(parsedMessage);
          }
        }
        return this;
      }

      private int int32Val_ ;
      /**
       * <code>int32 int32Val = 1;</code>
       * @return The int32Val.
       */
      @java.lang.Override
      public int getInt32Val() {
        return int32Val_;
      }
      /**
       * <code>int32 int32Val = 1;</code>
       * @param value The int32Val to set.
       * @return This builder for chaining.
       */
      public Builder setInt32Val(int value) {
        
        int32Val_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>int32 int32Val = 1;</code>
       * @return This builder for chaining.
       */
      public Builder clearInt32Val() {
        
        int32Val_ = 0;
        onChanged();
        return this;
      }

      private long int64Val_ ;
      /**
       * <code>int64 int64Val = 2;</code>
       * @return The int64Val.
       */
      @java.lang.Override
      public long getInt64Val() {
        return int64Val_;
      }
      /**
       * <code>int64 int64Val = 2;</code>
       * @param value The int64Val to set.
       * @return This builder for chaining.
       */
      public Builder setInt64Val(long value) {
        
        int64Val_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>int64 int64Val = 2;</code>
       * @return This builder for chaining.
       */
      public Builder clearInt64Val() {
        
        int64Val_ = 0L;
        onChanged();
        return this;
      }

      private float floatVal_ ;
      /**
       * <code>float floatVal = 3;</code>
       * @return The floatVal.
       */
      @java.lang.Override
      public float getFloatVal() {
        return floatVal_;
      }
      /**
       * <code>float floatVal = 3;</code>
       * @param value The floatVal to set.
       * @return This builder for chaining.
       */
      public Builder setFloatVal(float value) {
        
        floatVal_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>float floatVal = 3;</code>
       * @return This builder for chaining.
       */
      public Builder clearFloatVal() {
        
        floatVal_ = 0F;
        onChanged();
        return this;
      }

      private double doubleVal_ ;
      /**
       * <code>double doubleVal = 4;</code>
       * @return The doubleVal.
       */
      @java.lang.Override
      public double getDoubleVal() {
        return doubleVal_;
      }
      /**
       * <code>double doubleVal = 4;</code>
       * @param value The doubleVal to set.
       * @return This builder for chaining.
       */
      public Builder setDoubleVal(double value) {
        
        doubleVal_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>double doubleVal = 4;</code>
       * @return This builder for chaining.
       */
      public Builder clearDoubleVal() {
        
        doubleVal_ = 0D;
        onChanged();
        return this;
      }

      private java.lang.Object strVal_ = "";
      /**
       * <code>string strVal = 5;</code>
       * @return The strVal.
       */
      public java.lang.String getStrVal() {
        java.lang.Object ref = strVal_;
        if (!(ref instanceof java.lang.String)) {
          com.google.protobuf.ByteString bs =
              (com.google.protobuf.ByteString) ref;
          java.lang.String s = bs.toStringUtf8();
          strVal_ = s;
          return s;
        } else {
          return (java.lang.String) ref;
        }
      }
      /**
       * <code>string strVal = 5;</code>
       * @return The bytes for strVal.
       */
      public com.google.protobuf.ByteString
          getStrValBytes() {
        java.lang.Object ref = strVal_;
        if (ref instanceof String) {
          com.google.protobuf.ByteString b = 
              com.google.protobuf.ByteString.copyFromUtf8(
                  (java.lang.String) ref);
          strVal_ = b;
          return b;
        } else {
          return (com.google.protobuf.ByteString) ref;
        }
      }
      /**
       * <code>string strVal = 5;</code>
       * @param value The strVal to set.
       * @return This builder for chaining.
       */
      public Builder setStrVal(
          java.lang.String value) {
        if (value == null) {
    throw new NullPointerException();
  }
  
        strVal_ = value;
        onChanged();
        return this;
      }
      /**
       * <code>string strVal = 5;</code>
       * @return This builder for chaining.
       */
      public Builder clearStrVal() {
        
        strVal_ = getDefaultInstance().getStrVal();
        onChanged();
        return this;
      }
      /**
       * <code>string strVal = 5;</code>
       * @param value The bytes for strVal to set.
       * @return This builder for chaining.
       */
      public Builder setStrValBytes(
          com.google.protobuf.ByteString value) {
        if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
        
        strVal_ = value;
        onChanged();
        return this;
      }
      @java.lang.Override
      public final Builder setUnknownFields(
          final com.google.protobuf.UnknownFieldSet unknownFields) {
        return super.setUnknownFields(unknownFields);
      }

      @java.lang.Override
      public final Builder mergeUnknownFields(
          final com.google.protobuf.UnknownFieldSet unknownFields) {
        return super.mergeUnknownFields(unknownFields);
      }


      // @@protoc_insertion_point(builder_scope:com.alimin.fk.pb.FkPbValue)
    }

    // @@protoc_insertion_point(class_scope:com.alimin.fk.pb.FkPbValue)
    private static final com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue();
    }

    public static com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    private static final com.google.protobuf.Parser<FkPbValue>
        PARSER = new com.google.protobuf.AbstractParser<FkPbValue>() {
      @java.lang.Override
      public FkPbValue parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        return new FkPbValue(input, extensionRegistry);
      }
    };

    public static com.google.protobuf.Parser<FkPbValue> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<FkPbValue> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public com.alimin.fk.pb.FkPbValueOuterClass.FkPbValue getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_com_alimin_fk_pb_FkPbValue_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_com_alimin_fk_pb_FkPbValue_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n\017FkPbValue.proto\022\020com.alimin.fk.pb\"d\n\tF" +
      "kPbValue\022\020\n\010int32Val\030\001 \001(\005\022\020\n\010int64Val\030\002" +
      " \001(\003\022\020\n\010floatVal\030\003 \001(\002\022\021\n\tdoubleVal\030\004 \001(" +
      "\001\022\016\n\006strVal\030\005 \001(\tb\006proto3"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_com_alimin_fk_pb_FkPbValue_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_com_alimin_fk_pb_FkPbValue_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_com_alimin_fk_pb_FkPbValue_descriptor,
        new java.lang.String[] { "Int32Val", "Int64Val", "FloatVal", "DoubleVal", "StrVal", });
  }

  // @@protoc_insertion_point(outer_class_scope)
}
